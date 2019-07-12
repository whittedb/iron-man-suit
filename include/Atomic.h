#pragma once

static inline int disable_irq(void) {
    int primask;
    asm volatile("mrs %0, PRIMASK\n"
                    "cpsid i\n" : "=r"(primask));
    return primask & 1;
}

static inline void enable_irq() {
    asm volatile("cpsie i\n");
}

class Atomic {
    public:
    Atomic() {
        prevState = disable_irq();
    }
    ~Atomic() {
        if (prevState) {
            enable_irq();
        }
    }

    private:
    int prevState;
};
