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
![center](assets/overview.drawio.svg)
<!-- _class: small -->

> Learn how AI actually runs—efficiently, at scale, and within real-world constraints—by aligning machine learning models, hardware, and data.

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

- **Artificial Intelligence (AI)** is the broad goal
- **Machine Learning (ML)** is a subset of AI
- **Deep Learning (DL)** is a subset of ML
- Hardware progress often *unlocked* new AI paradigms

---

# Historical Evolution: 

**Symbolic Systems to Deep Learning. ML is part of AI**

---
# Primer NN: Perceptron, XOR Crisis, Training, and Inference

---
# MNIST and LeNet-5

---
# Takeaway: 

* Machines can learn from data using neural networks
* Hidden layer enable resolving nonlinear solutions for practical problems, e.g. MNIST
* Übung: O(n) to O(n^2) memory and Compute. Calculate on Paper and Verify with pytorch