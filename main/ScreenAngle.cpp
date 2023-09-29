//Функция ReliefFunction моделирует рельеф на определённом азимутальном углу
double ReliefFunction(double x)
{
    return cos(x) + x/M_PI + pow(x, 2/3);
}

//Функция FindCurrAngle ищет угол для нынешнего положения радиуса, возвращает угол в градусах
double FindCurrAngle(double x1, double y1, double x2, double y2)
{
    double opposite_cathet = y2 - y1;
    double adjacent_cathet = x2 - x1;
    return atan (opposite_cathet/adjacent_cathet) * 180.0 / M_PI;
}
