#ifndef OPTIONS_H
#define OPTIONS_H


class Options{

public:
    Options();
    void init();
    void getSigmaValue(int sigma[3]);

private:
    float sigma_MIN;
    float sigma_MAX;
    int sigma_Step;
    float alpha;
    float beta;
    float c;
};

#endif // OPTIONS_H
