//Функция ReliefFunction моделирует рельеф на определённом азимутальном углу
double ReliefFunction(double x)
{
    return cos(x) + x/M_PI + pow(x, 2/3);
}
