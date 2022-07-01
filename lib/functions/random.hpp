class RandomGenerator {
    public:
        void SetSeed(uint32_t seed);
        uint32_t Generate(uint32_t max);
        uint32_t m_x = 0;
        uint32_t m_mul = 0;
        uint32_t m_add = 0;
};

void RandomGenerator::SetSeed(uint32_t seed)
{
    this->m_x = seed;
    this->m_mul = 893457621;
    this->m_add = 198349821;
}

uint32_t RandomGenerator::Generate(uint32_t max = 100) {
    this->m_x ^= (this->m_x << 13);
    this->m_x ^= (this->m_x >> 17);
    this->m_x ^= (this->m_x << 5);
    this->m_x = (this->m_mul * this->m_x + this->m_add) % max;
    return this->m_x;
}

RandomGenerator* rng;
