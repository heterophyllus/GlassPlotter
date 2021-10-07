#ifndef AIR_H
#define AIR_H


class Air
{
public:
    Air();
    ~Air();

    static double refractive_index_abs(double lambdamicron, double T, double P= 101325.0);

private:
    static double refractive_index_ref(double lambdamicron);

};

#endif // AIR_H
