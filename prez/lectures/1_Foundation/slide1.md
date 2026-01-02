---
marp: true
theme: default
paginate: true

style: |
  section {
    justify-content: start;    
  }
  img {
    display: block;
    margin: 0 auto;
  }
---
![bg left:40% 80%](https://d.th-nuernberg.de/wordpress/wp-content/uploads/2023/03/ohm-logo-rot.png)
# **Foundation**


---

# What is the Lecture about?

<!-- SVG hier -->
![center](assets/diagrams/6overview.svg)
<!-- _class: small -->

> Learn how AI actually runs—efficiently, at scale, and within real-world constraints—by aligning machine learning models, **hardware**, and data.

---
# Why AI on Accelerators?
- Real-time perception (ADAS, autonomous driving)
- High data rates (cameras, radar, LiDAR)
- CPUs are too slow / power-hungry
- **Specialized accelerators** enable:
  - Low latency
  - High throughput
  - Energy efficiency

---

# Organization!
- 2+2 Lecture and Labs
- Moodle Course Link
  - See Course Description for Registration

---
# Fahrplan
<!-- _class: ascii -->
```text {style="font-size:0.75em; line-height:1;"}
                         ┌──────────────────────────┐
                         │      MACHINE LEARNING    │
                         │  (Models • Training • AI)│
                         └─────────────┬────────────┘
                                       │
                          ┌────────────▼────────────┐
                          │     AI ACCELERATORS     │
                          │     TPU • NPU • GPU     │
                          └────────────┬────────────┘
                                       │
        ┌──────────────────────────────▼──────────────────────────────-┐
        │                         FOUNDATIONS                          │
        │   ┌──────────┐   ┌──────────┐   ┌──────────┐   ┌──────────┐  │
        │   │  LINEAR  │   │  MEMORY  │   │ PARALLEL │   │  DATAFLOW│  │
        │   │ ALGEBRA  │   │  SYSTEMS │   │ COMPUTE  │   │  & TILING│  │
        │   └──────────┘   └──────────┘   └──────────┘   └──────────┘  │
        │   ┌──────────┐   ┌──────────┐   ┌──────────┐   ┌──────────┐  │
        │   │ FIXED &  │   │ PIPELINE │   │  SIMD /  │   │ ENERGY & │  │
        │   │ LOW PREC │   │  DESIGN  │   │  SYSTOLIC│   │  LATENCY │  │
        │   └──────────┘   └──────────┘   └──────────┘   └──────────┘  │
        └──────────────────────────────────────────────────────────────┘
```
---
# Personal Background: 

- Trained in **traditional software and systems engineering**
- Driver Assistance and Autonomous Driving
  - Watched handcrafted logic get **replaced by neural networks**

---
# Personal Background:

> **Sutton’s Bitter Lesson**: General-purpose learning methods that leverage computation ultimately dominate specialized, human-crafted solutions—given enough compute and data.

- Internalized **Sutton’s Bitter Lesson**
    - Compute Hardware + data + learning methods > human-crafted cleverness
- Shift in mindset:
    - **Systems engineering over human ingenuity**
    - Chess, Autonomous Driving — same pattern, different domains
---

# Big Picture
```
┌───────────────────────────────────────────────────────────────────────────┐
│                           ARTIFICIAL INTELLIGENCE                         │
│                    Early artificial intelligence excitement               │
│                                                                           │
│   ┌─────────────────────────────────────────────────────────────────────┐ │
│   │                           MACHINE LEARNING                          │ │
│   │                   Machine learning begins to flourish               │ │
│   │                                                                     │
│   │     ┌─────────────────────────────────────────────────────────────┐ │   │
│   │     │                         DEEP LEARNING                       │ │   │
│   │     │             Deep learning breakthroughs drive AI boom       │ │
│   │     └─────────────────────────────────────────────────────────────┘ │   │
│   │                                                                     │
│   └─────────────────────────────────────────────────────────────────────┘   │
│                                                                             │
└───────────────────────────────────────────────────────────────────────────┘
```

- **Artificial Intelligence (AI)** is the broad goal
- Hardware progress often *unlocked* new AI paradigms

---

# Historical Evolution:
## Era of AI Foundation
```
1950s        1960s        1970s        1980s        1990s        2000s        2010s
|------------|------------|------------|------------|------------|------------|
AI foundations        ML emerges & grows            Deep Learning surge
```
**Symbolic Systems**

---

# Historical Evolution:
## Era of Machine Learning
```
1950s        1960s        1970s        1980s        1990s        2000s        2010s
|------------|------------|------------|------------|------------|------------|
AI foundations        ML emerges & grows            Deep Learning surge
```

---

# What is Machine Learning (ML)?
> "A computer program is said to learn from **experience E** with respect to some class of **tasks T** and **performance measure P**, if its performance at tasks in T, as measured by P, improves with experience E" – **Tom Mitchell**

- **Tasks:** Classification (categories) or Regression (numerical values).
- **Performance:** Measured by Accuracy or Error (0-1 Loss).
- **Experience:** Supervised, Unsupervised, or Reinforcement Learning.
- [] Example E-mail Span

---
# Machine Learning: Training vs. Inference
<!-- SVG hier -->
![center](assets/diagrams/0ml.svg)
<!-- _class: small -->

- **Training**: Learn a hypothesis that maps inputs to outputs
- Generalize to unseen data
- **Goal**: enable **inference**
- Example: x? y?
---
## Dataset
> Example: MNIST dataset is a benchmark consisting of 70,000 grayscale images (28x28 pixels) of handwritten digits from 0 to 9, primarily used for training, validation, and testing image classification models
- Task: reliable handwritten digit recognition to automate tasks like sorting mail and processing bank checks.
> <!-- SVG hier -->
![center](assets/diagrams/1dataset.svg)
<!-- _class: small -->
- Dataset Split
  - **Training Set**: train the model
  - **Validation Set**: tune model and parameters
  - **Test Set**: final evaluation
---
# Model: 
- **Linear Classifier Model** is a special case of a hypothesis/model
- is a linear function consisting of model parameters (Weights and bias) which maps inputs, $x_i$ to class scores or probabilities
  $$
  f(x_i, W, b) = W x_i + b
  $$
- 32×32 RGB image Flattened to a input vector **x ∈ ℝ³⁰⁷²**
> <!-- SVG hier -->
![center](assets/diagrams/2classifier.svg)
<!-- _class: small -->
---
# Model: Linear Classifier Intuition
- Each image is a point in high-dimensional space
- Decision boundary is linear. Example: points on one side classified as “Car”
> <!-- SVG hier -->
![center](assets/linearclassifier-intuition.svg)
<!-- _class: small -->
---
# Learning Algorithm
> Find weights **W** and bias **b** such that: Correct class scores are higher than incorrect ones
> <!-- SVG hier -->
![center](assets/diagrams/3learning.svg)
<!-- _class: small -->

- **Loss function** measures penalty for wrong predictions => Lower loss = better model
- Parameter updates: Iterative approach
---
# Loss Function
- Example: Multiclass SVM Loss
- loss over single sample
$$
\mathcal{L}_i =
\sum_{j \ne y_i}
\max(0, s_j - s_{y_i} + 1)
$$
- Where $s_{y_i}$ = score of the correct class, $s_j$ = score of an incorrect class and Margin = 1
> <!-- SVG hier -->
![center](assets/diagrams/4loss.svg)
<!-- _class: small -->

<!-- 
Loss is zero only if the correct class score is larger than all others by at least 1
-->

---
# Loss Function
$$
Loss=\frac{1}{N}\sum\limits_{i} L_i
$$
- Loss over whole dataset
  - Average loss over all training samples
  - Perfect prediction → loss = 0
  - Otherwise → loss > 0
---
# Loss Function
- Softmax Classifier
- Outputs class probabilities
- Uses **Cross-Entropy Loss**
> <!-- SVG hier -->
![center](assets/diagrams/5softmax.svg)
<!-- _class: small -->
---
# Loss Function
- Total Loss

---
# Optimization Methods
- Random Search
- Gradient Descent
- Stochastic Gradient Descent (SGD)
---
# Takeaway: 

- Machines can learn from data
- 