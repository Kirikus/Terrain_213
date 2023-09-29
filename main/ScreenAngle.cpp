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

//Функция FindScreeningAngle ищет угол закрытия для заданного примерного рельефа, возвращает угол в градусах
double FindScreeningAngle(double azi, double R)
{
    double RLS_x = 2.5*M_PI, RLS_y = ReliefFunction(RLS_x);
    double screening_angle = 0;
    double dx = 0.05;
    double max_angle = 0;
    for (double curr_x = RLS_x + dx; curr_x <= RLS_x+ R; curr_x+=dx)
    {
        double curr_y = ReliefFunction(curr_x);
        if (curr_y <= RLS_y)
            continue;

        double curr_angle = FindCurrAngle(RLS_x, RLS_y, curr_x, curr_y);
        if (curr_angle <= max_angle)
            continue;

        max_angle = curr_angle;
        screening_angle = max_angle;
    }

    return screening_angle;
}
