#include <algorithm>
#include <iostream>

using namespace std;

struct F3 {
  double a, b, c;
  F3() : a(0), b(0), c(0) {}
};

F3 temp(double t) {
  F3 m;
  if (t <= 10)
    m.a = 1;
  else if (t < 15)
    m.a = (15 - t) / 5;
  if (t > 10 && t <= 22.5)
    m.b = (t - 10) / 12.5;
  else if (t > 22.5 && t < 30)
    m.b = (30 - t) / 7.5;
  if (t > 25 && t < 30)
    m.c = (t - 25) / 5;
  else if (t >= 30)
    m.c = 1;
  return m;
}

F3 hum(double h) {
  F3 m;
  if (h <= 20)
    m.a = 1;
  else if (h < 30)
    m.a = (30 - h) / 10;
  if (h > 25 && h <= 45)
    m.b = (h - 25) / 20;
  else if (h > 45 && h < 60)
    m.b = (60 - h) / 15;
  if (h > 55 && h < 60)
    m.c = (h - 55) / 5;
  else if (h >= 60)
    m.c = 1;
  return m;
}

F3 rules(F3 t, F3 h) {
  F3 f;
  f.a = max({min(t.a, h.a), min(t.a, h.b), min(t.b, h.a)});
  f.b = max({min(t.b, h.b), min(t.b, h.c), min(t.c, h.a), min(t.a, h.c)});
  f.c = max({min(t.c, h.b), min(t.c, h.c), min(t.b, h.c)});
  return f;
}

double defuzz(F3 f) {
  double d = f.a + f.b + f.c;
  return d ? (25 * f.a + 45 * f.b + 70 * f.c) / d : 30;
}

void test(int n, double t, double h) {
  double s = defuzz(rules(temp(t), hum(h)));
  cout << "\nCase " << n << ": Temp=" << t << "C Hum=" << h
       << "% => FanSpeed=" << s << " ("
       << (s < 35   ? "Slow"
           : s < 60 ? "Medium"
                    : "Fast")
       << ")" << endl;
}

int main() {
  cout << "Fuzzy Room Temperature Control\n";
  cout << "===============================" << endl;

  test(1, 8, 15);
  test(2, 22, 45);
  test(3, 35, 75);
  test(4, 25, 65);
  test(5, 32, 25);

  return 0;
}