#include <algorithm>
#include <iostream>

using namespace std;

struct Exam {
  double poor, average, good;
  Exam() : poor(0), average(0), good(0) {}
};

struct FuzzySet {
  double low, medium, high;
  FuzzySet() : low(0), medium(0), high(0) {}
};

struct Performance {
  double weak, moderate, strong;
  Performance() : weak(0), moderate(0), strong(0) {}
};

Exam getExamMembership(double score) {
  Exam m;
  if (score >= 0 && score <= 35)
    m.poor = 1.0;
  else if (score > 35 && score < 50)
    m.poor = (50 - score) / 15;

  if (score > 40 && score <= 60)
    m.average = (score - 40) / 20;
  else if (score > 60 && score < 80)
    m.average = (80 - score) / 20;

  if (score > 70 && score < 85)
    m.good = (score - 70) / 15;
  else if (score >= 85 && score <= 100)
    m.good = 1.0;

  return m;
}

FuzzySet getParticipationMembership(double p) {
  FuzzySet m;
  if (p >= 0 && p <= 30)
    m.low = 1.0;
  else if (p > 30 && p < 50)
    m.low = (50 - p) / 20;

  if (p > 40 && p <= 60)
    m.medium = (p - 40) / 20;
  else if (p > 60 && p < 80)
    m.medium = (80 - p) / 20;

  if (p > 70 && p < 85)
    m.high = (p - 70) / 15;
  else if (p >= 85 && p <= 100)
    m.high = 1.0;

  return m;
}

FuzzySet getAssignmentMembership(double a) {
  FuzzySet m;
  if (a >= 0 && a <= 30)
    m.low = 1.0;
  else if (a > 30 && a < 50)
    m.low = (50 - a) / 20;

  if (a > 40 && a <= 60)
    m.medium = (a - 40) / 20;
  else if (a > 60 && a < 80)
    m.medium = (80 - a) / 20;

  if (a > 70 && a < 85)
    m.high = (a - 70) / 15;
  else if (a >= 85 && a <= 100)
    m.high = 1.0;

  return m;
}

Performance evaluateRules(Exam e, FuzzySet p, FuzzySet a) {
  Performance perf;

  perf.weak =
      max({min({e.poor, p.low, a.low}), min({e.poor, p.medium, a.low}),
           min({e.poor, p.low, a.medium}), min({e.average, p.low, a.low})});

  perf.moderate = max(
      {min({e.average, p.medium, a.medium}), min({e.average, p.high, a.medium}),
       min({e.average, p.medium, a.high}), min({e.good, p.low, a.low})});

  perf.strong =
      max({min({e.good, p.high, a.high}), min({e.good, p.medium, a.high}),
           min({e.good, p.high, a.medium})});

  return perf;
}

double defuzzify(Performance p) {
  double num = 25 * p.weak + 55 * p.moderate + 85 * p.strong;
  double den = p.weak + p.moderate + p.strong;
  return (den == 0) ? 0 : num / den;
}

void test(int n, double exam, double part, double assign) {
  cout << "\nStudent " << n << ": Exam=" << exam << " Part=" << part
       << " Assign=" << assign;

  Exam e = getExamMembership(exam);
  FuzzySet p = getParticipationMembership(part);
  FuzzySet a = getAssignmentMembership(assign);
  Performance perf = evaluateRules(e, p, a);
  double score = defuzzify(perf);

  cout << " => Performance=" << score << " (";
  if (score < 40)
    cout << "Weak";
  else if (score < 70)
    cout << "Moderate";
  else
    cout << "Strong";
  cout << ")" << endl;
}

int main() {
  cout << "Fuzzy Student Performance Evaluation\n";
  cout << "=====================================" << endl;

  test(1, 30, 20, 25);
  test(1, 30, 20, 25);
  test(2, 60, 55, 65);
  test(3, 90, 95, 92);
  test(4, 85, 35, 40);
  test(5, 70, 80, 75);

  return 0;
}