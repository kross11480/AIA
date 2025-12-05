#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_SIZE (8*1024*1024) //8MB

typedef struct
{
    uint8_t *base;
    size_t size;
    size_t offset;
} Arena;

static Arena arena;
static uint8_t arena_buffer[ARENA_SIZE];

static inline size_t align4(size_t x)
{
    return (x + 3) & ~((size_t) 0x3);
}

void init_arena()
{
    arena.base = arena_buffer;
    arena.size = ARENA_SIZE;
    arena.offset = 0;
}

void *alloc_arena(size_t size)
{
    size_t start = align4(arena.offset);
    size_t end   = start + size;
    if (end > ARENA_SIZE)
    {
        return NULL;
    }
    arena.offset = end;
    void *ptr = arena.base + start;
    return ptr;
}

void usage_arena()
{
    printf("Usage arena size: %d\n", (arena.offset) );
}

typedef struct
{
    uint8_t *pixel_buffer;
    uint32_t width;
    uint32_t height;
} Image;

typedef struct
{
    Image *images;
    uint8_t *labels;
    uint32_t count;
} ImageBatch;

#define FILE_HEADER_STRING "__info2_image_file_format__"
#define FILE_HEADER_MODEL "__info2_neural_network_file_format__"
#define BUFFER_SIZE 100

ImageBatch *get_images_batch()
{
    FILE *fp = fopen("../code/data/mnist_test.info2", "rb");
    char buf[BUFFER_SIZE];
    if (fp == NULL)
    {
        printf("open file fail\n");
    }

    fread(buf, sizeof(char), strlen(FILE_HEADER_STRING), fp);
    if (strcmp(FILE_HEADER_STRING, buf) != 0)
    {
        printf("File has invalid header!\n");
    }

    uint16_t count = 0;
    fread(&count, sizeof(uint16_t), 1, fp);
    uint16_t width = 0;
    fread(&width, sizeof(uint16_t), 1, fp);
    uint16_t height = 0;
    fread(&height, sizeof(uint16_t), 1, fp);

    ImageBatch *batch = (ImageBatch *)alloc_arena(sizeof(ImageBatch));

    uint32_t test_count = 10;
    batch->images = (Image *) alloc_arena(sizeof(Image) * test_count);
    batch->labels = (uint8_t *)alloc_arena(sizeof(uint8_t) * test_count);
    batch->count = 0;
    for (int i = 0; i < test_count; i++)
    {
        //create image
        Image image = {NULL, width, height};
        image.pixel_buffer = (uint8_t *) alloc_arena(sizeof(uint8_t) * image.width * image.height);
        //read image
        int numbytes = fread(image.pixel_buffer, sizeof(uint8_t), image.width * image.height, fp);
        batch->images[i] = image;

        //read label
        uint8_t label;
        fread(&label, sizeof(uint8_t), 1, fp);
        batch->labels[i] = label;
        printf("label[%d] = %d\n", i, batch->labels[i]);
        batch->count += 1;
    }
    fclose(fp);
    return batch;
}

void softmax(float *x, int dim)
{
    //find max val (log sum exp trick)
    float max_val = 0, sum = 0;
    for (int i = 0; i < dim; i++)
    {
        max_val = max_val < x[i] ? x[i] : max_val;
    }
    //calculate denominator
    for (int i = 0; i < dim; i++)
    {
        x[i] = expf(x[i] - max_val);
        sum = sum + x[i];
    }
    //softmax
    for (int i = 0; i < dim; i++)
    {
        x[i] = x[i] / sum;
    }
}

void relu(float *x, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        x[i] = (x[i] < 0) ? 0 : x[i];
    }
}

/*y(d, )  = w(d,n) * x(n, ) */
void matmul(float *y, float *x, float *w, int d, int n)
{
    for (int i = 0; i < d; i++)
    {
        float val = 0.0f;
        for (int j = 0; j < n; j++)
        {
            val += w[i * n + j] * x[j];
        }
        y[i] = val;
    }
}

/*y(d, )  = y(d,) + x(d, ) */
void vecadd(float *y, float *x, int d)
{
    for (int i = 0; i < d; i++)
    {
        y[i] += x[i];
    }
}

int argmax(float *x, int d)
{
    int max_index = 0;
    float argmaxval = 0.0f;
    for (int i = 0; i < d; i++)
    {
        if (argmaxval < x[i])
        {
            argmaxval = x[i];
            max_index = i;
        }
    }
    return max_index;
}

typedef struct {
    int dim; // dimension
    int n_layers; // number of layers
} Config;

typedef struct
{
    int n1;
    int d1;
    float *w1;
    float *b1;
    int n2;
    int d2;
    float *w2;
    float *b2;
    int n3;
    int d3;
    float *w3;
    float *b3;
} Weights;

typedef struct {
    // Input buffer (flattened MNIST image 28x28 = 784)
    float *x;       // (784)
    // Hidden activation buffers
    float *h1;      // (200)  after Dense 1
    float *h2;      // (100)  after Dense 2
    // Output logits
    float *logits;  // (10)
} RunState;

void read_checkpoint(Weights *weights)
{
    FILE *fp = fopen("../code/data/mnist_model.info2", "rb");
    char buf[BUFFER_SIZE];
    fread(buf, sizeof(char), strlen(FILE_HEADER_MODEL), fp);
    printf("%s\n", buf);
    if (strcmp(FILE_HEADER_MODEL, buf) != 0)
    {
        printf("File has invalid header!\n");
    }
    //read dimensions
    int input_dim = 0;
    fread(&input_dim, sizeof(int), 1, fp);
    int output_dim = 0;
    fread(&output_dim, sizeof(int), 1, fp);


    int rows = output_dim;
    int cols = input_dim;
    weights->n1 = rows;
    weights->d1 = cols;
    weights->w1 = (float *)alloc_arena(rows * cols * sizeof(float));
    fread(weights->w1, sizeof(float), rows * cols, fp);
    weights->b1 = (float *)alloc_arena(rows * sizeof(float));
    fread(weights->b1, sizeof(float), rows, fp);

    input_dim = output_dim;
    fread(&output_dim, sizeof(int), 1, fp);
    rows = output_dim;
    cols = input_dim;
    weights->n2 = rows;
    weights->d2 = cols;
    weights->w2 = (float *)alloc_arena(rows * cols * sizeof(float));
    fread(weights->w2, sizeof(float), rows * cols, fp);
    weights->b2 = (float *)alloc_arena(rows * sizeof(float));
    fread(weights->b2, sizeof(float), rows, fp);

    input_dim = output_dim;
    fread(&output_dim, sizeof(int), 1, fp);
    rows = output_dim;
    cols = input_dim;
    weights->n3 = rows;
    weights->d3 = cols;
    weights->w3 = (float *)alloc_arena(rows * cols * sizeof(float));
    fread(weights->w3, sizeof(float), rows * cols, fp);
    weights->b3 = (float *)alloc_arena(rows * sizeof(float));
    fread(weights->b3, sizeof(float), rows, fp);
}

void test();

void u8_to_float(uint8_t *pixel_buffer, float *x, int count)
{
    for (int i = 0; i < count; i++){
        x[i] = (float) pixel_buffer[i];
    }
}

int main()
{
    init_arena();
    //read test images with labels
    ImageBatch *batch = get_images_batch();
    //read model
    Weights *weights = (Weights *)alloc_arena(sizeof(Weights));
    read_checkpoint(weights);

    RunState st;

    Image input_img = batch->images[5];
    printf("%d\n", input_img.width);
    st.x = (float *) alloc_arena(input_img.width * input_img.height * sizeof(float));
    u8_to_float(input_img.pixel_buffer, (float *)st.x, input_img.width * input_img.height);

    st.h1 = (float *) alloc_arena(weights->d2 * sizeof(float));
    matmul(st.h1, st.x, weights->w1, weights->n1, weights->d1);
    vecadd(st.h1, weights->b1, weights->d2);
    relu(st.h1, weights->d2);

    st.h2 = (float *) alloc_arena(weights->d3 * sizeof(float));
    matmul(st.h2, st.h1, weights->w2, weights->n2, weights->d2);
    vecadd(st.h2, weights->b2, weights->d3);
    relu(st.h2, weights->d3);

    st.logits = (float *) alloc_arena(weights->n3 * sizeof(float));
    matmul(st.logits, st.h2, weights->w3, weights->n3, weights->d3);
    vecadd(st.logits, weights->b3, weights->n3);
    softmax(st.logits, weights->n3);

    printf("Predicted Class: %d \n\n", argmax(st.logits, weights->n3));
}

void test()
{
    float *test = (float *)alloc_arena(sizeof(float));
    float test_val[] = {2, 1.0, 0.5};
    //memcpy(test, test_val, 3 * sizeof(float));
    //softmax(test, 3);

    //float test_val[] = {2, 1.0, -0.5};
    //memcpy(test, test_val, 3 * sizeof(float));
    //relu(test, 3);
    int n = 4;
    int d = 3;
    float *test_a = (float *)alloc_arena(d * n * sizeof(float));
    float *test_b = (float *)alloc_arena(n * sizeof(float));
    float *test_c = (float *)alloc_arena(d * sizeof(float));

    float w[12] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12
    };
    memcpy(test_a, w, d *n * sizeof(float));
    float x[4] = {1, 2, 3, 4};
    memcpy(test_b, x, n * sizeof(float));
    matmul(test_c,test_b,test_a, d, n);

    // print results
    printf("y = [");
    for (int i = 0; i < d; i++)
    {
        printf(" %f ", test_c[i]);
    }
    printf("]\n");
}