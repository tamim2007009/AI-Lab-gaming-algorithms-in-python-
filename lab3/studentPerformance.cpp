#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

struct FuzzySet3 {
  double low;
  double medium;
  double high;
  FuzzySet3() : low(0.0), medium(0.0), high(0.0) {}
};

struct ExamScoreMembership {
  double poor;
  double average;
  double good;
  ExamScoreMembership() : poor(0.0), average(0.0), good(0.0) {}
};

struct PerformanceMembership {
  double weak;
  double moderate;
  double strong;
  PerformanceMembership() : weak(0.0), moderate(0.0), strong(0.0) {}
};

// Fuzzification for Exam Score (0-100): Poor, Average, Good
ExamScoreMembership getExamScoreMembership(double score) {
  ExamScoreMembership membership;

  // Poor (0-50): Trapezoidal - full from 0-35, slopes down to 50
  if (score >= 0 && score <= 35) {
    membership.poor = 1.0;
  } else if (score > 35 && score < 50) {
    membership.poor = (50 - score) / (50 - 35);
  }

  // Average (40-80): Triangular - slopes up from 40, peaks at 60, down to 80
  if (score > 40 && score <= 60) {
    membership.average = (score - 40) / (60 - 40);
  } else if (score > 60 && score < 80) {
    membership.average = (80 - score) / (80 - 60);
  }

  // Good (70-100): Trapezoidal - slopes up from 70, full from 85-100
  if (score > 70 && score < 85) {
    membership.good = (score - 70) / (85 - 70);
  } else if (score >= 85 && score <= 100) {
    membership.good = 1.0;
  }

  return membership;
}

// Fuzzification for Class Participation (0-100): Low, Medium, High
FuzzySet3 getClassParticipationMembership(double participation) {
  FuzzySet3 membership;

  // Low (0-50): Trapezoidal - full from 0-30, slopes down to 50
  if (participation >= 0 && participation <= 30) {
    membership.low = 1.0;
  } else if (participation > 30 && participation < 50) {
    membership.low = (50 - participation) / (50 - 30);
  }

  // Medium (40-80): Triangular - slopes up from 40, peaks at 60, down to 80
  if (participation > 40 && participation <= 60) {
    membership.medium = (participation - 40) / (60 - 40);
  } else if (participation > 60 && participation < 80) {
    membership.medium = (80 - participation) / (80 - 60);
  }

  // High (70-100): Trapezoidal - slopes up from 70, full from 85-100
  if (participation > 70 && participation < 85) {
    membership.high = (participation - 70) / (85 - 70);
  } else if (participation >= 85 && participation <= 100) {
    membership.high = 1.0;
  }

  return membership;
}

// Fuzzification for Assignment Quality (0-100): Low, Medium, High
FuzzySet3 getAssignmentQualityMembership(double quality) {
  FuzzySet3 membership;

  // Low (0-50): Trapezoidal - full from 0-30, slopes down to 50
  if (quality >= 0 && quality <= 30) {
    membership.low = 1.0;
  } else if (quality > 30 && quality < 50) {
    membership.low = (50 - quality) / (50 - 30);
  }

  // Medium (40-80): Triangular - slopes up from 40, peaks at 60, down to 80
  if (quality > 40 && quality <= 60) {
    membership.medium = (quality - 40) / (60 - 40);
  } else if (quality > 60 && quality < 80) {
    membership.medium = (80 - quality) / (80 - 60);
  }

  // High (70-100): Trapezoidal - slopes up from 70, full from 85-100
  if (quality > 70 && quality < 85) {
    membership.high = (quality - 70) / (85 - 70);
  } else if (quality >= 85 && quality <= 100) {
    membership.high = 1.0;
  }

  return membership;
}

// Rule evaluation: 9 rules covering all major combinations
PerformanceMembership evaluateRules(ExamScoreMembership exam,
                                    FuzzySet3 participation,
                                    FuzzySet3 assignment) {
  PerformanceMembership performance;

  // Rule 1: IF Exam=Poor AND Participation=Low AND Assignment=Low THEN
  // Performance=Weak
  double rule1 = min({exam.poor, participation.low, assignment.low});
  performance.weak = max(performance.weak, rule1);

  // Rule 2: IF Exam=Poor AND Participation=Medium AND Assignment=Low THEN
  // Performance=Weak
  double rule2 = min({exam.poor, participation.medium, assignment.low});
  performance.weak = max(performance.weak, rule2);

  // Rule 3: IF Exam=Poor AND Participation=Low AND Assignment=Medium THEN
  // Performance=Weak
  double rule3 = min({exam.poor, participation.low, assignment.medium});
  performance.weak = max(performance.weak, rule3);

  // Rule 4: IF Exam=Average AND Participation=Medium AND Assignment=Medium THEN
  // Performance=Moderate
  double rule4 = min({exam.average, participation.medium, assignment.medium});
  performance.moderate = max(performance.moderate, rule4);

  // Rule 5: IF Exam=Average AND Participation=High AND Assignment=Medium THEN
  // Performance=Moderate
  double rule5 = min({exam.average, participation.high, assignment.medium});
  performance.moderate = max(performance.moderate, rule5);

  // Rule 6: IF Exam=Average AND Participation=Medium AND Assignment=High THEN
  // Performance=Moderate
  double rule6 = min({exam.average, participation.medium, assignment.high});
  performance.moderate = max(performance.moderate, rule6);

  // Rule 7: IF Exam=Good AND Participation=High AND Assignment=High THEN
  // Performance=Strong
  double rule7 = min({exam.good, participation.high, assignment.high});
  performance.strong = max(performance.strong, rule7);

  // Rule 8: IF Exam=Good AND Participation=Medium AND Assignment=High THEN
  // Performance=Strong
  double rule8 = min({exam.good, participation.medium, assignment.high});
  performance.strong = max(performance.strong, rule8);

  // Rule 9: IF Exam=Good AND Participation=High AND Assignment=Medium THEN
  // Performance=Strong
  double rule9 = min({exam.good, participation.high, assignment.medium});
  performance.strong = max(performance.strong, rule9);

  // Additional rules for better coverage
  // Rule 10: IF Exam=Poor OR (Participation=Low AND Assignment=Low) THEN
  // Performance=Weak
  double rule10 = max({exam.poor, min(participation.low, assignment.low)});
  performance.weak = max(performance.weak, rule10);

  // Rule 11: IF Exam=Average AND Participation=Low AND Assignment=Low THEN
  // Performance=Weak
  double rule11 = min({exam.average, participation.low, assignment.low});
  performance.weak = max(performance.weak, rule11);

  // Rule 12: IF Exam=Good AND Participation=Low AND Assignment=Low THEN
  // Performance=Moderate
  double rule12 = min({exam.good, participation.low, assignment.low});
  performance.moderate = max(performance.moderate, rule12);

  return performance;
}

// Defuzzification using Centroid method
double defuzzifyCentroid(PerformanceMembership performance) {
  // Define center points for output fuzzy sets (0-100 scale)
  // Weak: 0-40, center at 25
  // Moderate: 35-75, center at 55
  // Strong: 65-100, center at 85

  double weakCenter = 25.0;
  double moderateCenter = 55.0;
  double strongCenter = 85.0;

  double numerator = 0.0;
  double denominator = 0.0;

  if (performance.weak > 0) {
    numerator += weakCenter * performance.weak;
    denominator += performance.weak;
  }

  if (performance.moderate > 0) {
    numerator += moderateCenter * performance.moderate;
    denominator += performance.moderate;
  }

  if (performance.strong > 0) {
    numerator += strongCenter * performance.strong;
    denominator += performance.strong;
  }

  if (denominator == 0) {
    return 0.0;
  }

  return numerator / denominator;
}

// Test a single student
void testStudent(int studentNum, string name, double examScore,
                 double participation, double assignment) {
  cout << "\n========================================" << endl;
  cout << "STUDENT " << studentNum << ": " << name << endl;
  cout << "========================================" << endl;
  cout << fixed << setprecision(2);

  // Input
  cout << "\nInput Scores:" << endl;
  cout << "  Exam Score: " << examScore << "/100" << endl;
  cout << "  Class Participation: " << participation << "/100" << endl;
  cout << "  Assignment Quality: " << assignment << "/100" << endl;

  // Fuzzification
  ExamScoreMembership examMembership = getExamScoreMembership(examScore);
  FuzzySet3 participationMembership =
      getClassParticipationMembership(participation);
  FuzzySet3 assignmentMembership = getAssignmentQualityMembership(assignment);

  cout << "\nFuzzification:" << endl;
  cout << "  Exam Score:" << endl;
  cout << "    Poor: " << examMembership.poor << endl;
  cout << "    Average: " << examMembership.average << endl;
  cout << "    Good: " << examMembership.good << endl;

  cout << "  Class Participation:" << endl;
  cout << "    Low: " << participationMembership.low << endl;
  cout << "    Medium: " << participationMembership.medium << endl;
  cout << "    High: " << participationMembership.high << endl;

  cout << "  Assignment Quality:" << endl;
  cout << "    Low: " << assignmentMembership.low << endl;
  cout << "    Medium: " << assignmentMembership.medium << endl;
  cout << "    High: " << assignmentMembership.high << endl;

  // Rule evaluation
  PerformanceMembership performanceMembership = evaluateRules(
      examMembership, participationMembership, assignmentMembership);

  cout << "\nPerformance Membership:" << endl;
  cout << "  Weak: " << performanceMembership.weak << endl;
  cout << "  Moderate: " << performanceMembership.moderate << endl;
  cout << "  Strong: " << performanceMembership.strong << endl;

  // Defuzzification
  double finalPerformance = defuzzifyCentroid(performanceMembership);

  cout << "\nFinal Performance Score: " << finalPerformance << "/100" << endl;

  // Interpretation
  string grade;
  if (finalPerformance < 40) {
    grade = "WEAK (Needs Improvement)";
  } else if (finalPerformance < 70) {
    grade = "MODERATE (Satisfactory)";
  } else {
    grade = "STRONG (Excellent)";
  }
  cout << "Evaluation: " << grade << endl;
}

int main() {
  cout << "=====================================================" << endl;
  cout << "  FUZZY STUDENT PERFORMANCE EVALUATION SYSTEM       " << endl;
  cout << "=====================================================" << endl;

  cout << "\nSystem Configuration:" << endl;
  cout << "---------------------" << endl;
  cout << "Inputs:" << endl;
  cout << "  1. Exam Score (0-100): Poor (0-50), Average (40-80), Good (70-100)"
       << endl;
  cout << "  2. Class Participation (0-100): Low (0-50), Medium (40-80), High "
          "(70-100)"
       << endl;
  cout << "  3. Assignment Quality (0-100): Low (0-50), Medium (40-80), High "
          "(70-100)"
       << endl;
  cout << "\nOutput:" << endl;
  cout
      << "  Performance (0-100): Weak (0-40), Moderate (35-75), Strong (65-100)"
      << endl;

  cout << "\nFuzzy Rules (12 rules):" << endl;
  cout << "-----------------------" << endl;
  cout << "1. Poor Exam + Low Participation + Low Assignment → Weak" << endl;
  cout << "2. Poor Exam + Medium Participation + Low Assignment → Weak" << endl;
  cout << "3. Poor Exam + Low Participation + Medium Assignment → Weak" << endl;
  cout
      << "4. Average Exam + Medium Participation + Medium Assignment → Moderate"
      << endl;
  cout << "5. Average Exam + High Participation + Medium Assignment → Moderate"
       << endl;
  cout << "6. Average Exam + Medium Participation + High Assignment → Moderate"
       << endl;
  cout << "7. Good Exam + High Participation + High Assignment → Strong"
       << endl;
  cout << "8. Good Exam + Medium Participation + High Assignment → Strong"
       << endl;
  cout << "9. Good Exam + High Participation + Medium Assignment → Strong"
       << endl;
  cout << "10. Poor Exam OR (Low Participation AND Low Assignment) → Weak"
       << endl;
  cout << "11. Average Exam + Low Participation + Low Assignment → Weak"
       << endl;
  cout << "12. Good Exam + Low Participation + Low Assignment → Moderate"
       << endl;

  cout << "\n\n=====================================================" << endl;
  cout << "           TESTING WITH 5 SAMPLE STUDENTS           " << endl;
  cout << "=====================================================" << endl;

  // Test Case 1: Poor student
  testStudent(1, "Alice", 30.0, 20.0, 25.0);

  // Test Case 2: Average student
  testStudent(2, "Bob", 60.0, 55.0, 65.0);

  // Test Case 3: Excellent student
  testStudent(3, "Charlie", 90.0, 95.0, 92.0);

  // Test Case 4: Good exam but poor participation
  testStudent(4, "Diana", 85.0, 35.0, 40.0);

  // Test Case 5: Mixed performance
  testStudent(5, "Eve", 70.0, 80.0, 75.0);

  cout << "\n=====================================================" << endl;
  cout << "                  SUMMARY REPORT                    " << endl;
  cout << "=====================================================" << endl;
  cout << "\nThe fuzzy evaluation system successfully assessed 5 students"
       << endl;
  cout << "based on exam scores, class participation, and assignment quality."
       << endl;
  cout << "\nKey Findings:" << endl;
  cout << "- Students with consistently low scores across all metrics" << endl;
  cout << "  received WEAK performance ratings." << endl;
  cout << "- Students with balanced average scores received MODERATE ratings."
       << endl;
  cout << "- Students excelling in all areas received STRONG ratings." << endl;
  cout << "- The system considers all three inputs holistically using" << endl;
  cout << "  12 fuzzy rules and centroid defuzzification." << endl;
  cout << "\nConclusion:" << endl;
  cout << "The fuzzy logic system provides nuanced student evaluations" << endl;
  cout << "that account for uncertainty and multiple performance factors."
       << endl;
  cout << "=====================================================" << endl;

  return 0;
}
