#include <ScreenAngle.h>

/*
Функция ReliefFunction моделирует рельеф на определённом азимутальном углу
 */
double ReliefFunction(double x)
{
    return cos(x) + x/M_PI + pow(x, 2/3);
}

/*
Функция FindCurrAngle ищет угол для нынешнего положения радиуса, возвращает угол в градусах
Ищет тангенс угла через отношение катетов и возвращает угол в градусах
Принимает координаты РЛС и точки, которая считается самой высокой для заданного радиуса
*/
double FindCurrAngle(double x1, double y1, double x2, double y2)
{
    double opposite_cathet = y2 - y1;
    double adjacent_cathet = x2 - x1;
    return atan (opposite_cathet/adjacent_cathet) * 180.0 / M_PI;
}

/*
Функция FindScreeningAngle ищет угол закрытия для заданного примерного рельефа, возвращает угол в градусах
Принимает:
 azi - азимутальный угол (указывается в градусах);
 R - радиус поиска угла закрытия (указывается в метрах).
Проходится по непрерывной функции рельефа
*/
double FindScreeningAngle(double azi, double R)
{
    double rls_x = 2.5*M_PI, rls_y = ReliefFunction(rls_x);
    double screening_angle = 0;
    double dx = 0.05;
    double max_angle = 0;
    for (double curr_x = rls_x + dx; curr_x <= rls_x+ R; curr_x+=dx)
    {
        double curr_y = ReliefFunction(curr_x);
        if (curr_y <= rls_y)
            continue;

        double curr_angle = FindCurrAngle(rls_x, rls_y, curr_x, curr_y);
        if (curr_angle <= max_angle)
            continue;

        max_angle = curr_angle;
        screening_angle = max_angle;
    }

    return screening_angle;
}
