#include <algorithm>
#include <iostream>

using namespace std;

struct F3 {
  double a, b, c;
  F3() : a(0), b(0), c(0) {}
};

F3 density(double d) {
  F3 m;
  if (d <= 25)
    m.a = 1;
  else if (d < 40)
    m.a = (40 - d) / 15;
  if (d > 30 && d <= 50)
    m.b = (d - 30) / 20;
  else if (d > 50 && d < 70)
    m.b = (70 - d) / 20;
  if (d > 60 && d < 75)
    m.c = (d - 60) / 15;
  else if (d >= 75)
    m.c = 1;
  return m;
}

F3 wait(double w) {
  F3 m;
  if (w <= 20)
    m.a = 1;
  else if (w < 40)
    m.a = (40 - w) / 20;
  if (w > 30 && w <= 55)
    m.b = (w - 30) / 25;
  else if (w > 55 && w < 80)
    m.b = (80 - w) / 25;
  if (w > 70 && w < 90)
    m.c = (w - 70) / 20;
  else if (w >= 90)
    m.c = 1;
  return m;
}

F3 rules(F3 d, F3 w) {
  F3 g;
  g.a = max({min(d.a, w.a), min(d.a, w.b), min(d.b, w.a)});
  g.b = max({min(d.b, w.b), min(d.b, w.c), min(d.c, w.a), min(d.a, w.c)});
  g.c = max({min(d.c, w.b), min(d.c, w.c)});
  return g;
}

double defuzz(F3 g) {
  double total = g.a + g.b + g.c;
  return total ? (25 * g.a + 45 * g.b + 70 * g.c) / total : 30;
}

void test(int n, double d, double w) {
  double s = defuzz(rules(density(d), wait(w)));
  cout << "\nScenario " << n << ": Density=" << d << " Wait=" << w
       << "s => GreenLight=" << s << "s ("
       << (s < 35   ? "Short"
           : s < 60 ? "Medium"
                    : "Long")
       << ")" << endl;
}

int main() {
  cout << "Fuzzy Traffic Light Control\n";
  cout << "============================" << endl;

  test(1, 15, 10);
  test(2, 50, 55);
  test(3, 85, 100);
  test(4, 80, 25);
  test(5, 20, 95);
  test(6, 45, 35);

  return 0;
}
