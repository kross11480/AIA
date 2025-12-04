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
        numbytes = fread(&label, sizeof(uint8_t), 1, fp);
        batch->labels[i] = label;

        batch->count += 1;
    }
    fclose(fp);
    return batch;
}


int main()
{
    init_arena();
    //read test images with labels
    ImageBatch *batch = get_images_batch();
    //read model

}
