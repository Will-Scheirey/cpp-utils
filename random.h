#include <random>

struct Random {
    static float random() {
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_real_distribution<float> distr(0, 1);
        return distr(generator);
    }

    static float random(float max) {
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_real_distribution<float> distr(0, max);
        return distr(generator);
    }

    static float random(float min, float max) {
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_real_distribution<float> distr(min, max);
        return distr(generator);
    }
};