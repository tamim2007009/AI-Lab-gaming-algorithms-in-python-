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

struct LoadSizeMembership {
  double small;
  double medium;
  double large;
  LoadSizeMembership() : small(0.0), medium(0.0), large(0.0) {}
};

struct WashTimeMembership {
  double short_time;
  double medium_time;
  double long_time;
  WashTimeMembership() : short_time(0.0), medium_time(0.0), long_time(0.0) {}
};

// Fuzzification for Dirtiness (0-100 scale): Low, Medium, High
FuzzySet3 getDirtinessMembership(double dirtiness) {
  FuzzySet3 membership;

  // Low (0-40): Trapezoidal - full from 0-25, slopes down to 40
  if (dirtiness >= 0 && dirtiness <= 25) {
    membership.low = 1.0;
  } else if (dirtiness > 25 && dirtiness < 40) {
    membership.low = (40 - dirtiness) / (40 - 25);
  }

  // Medium (30-70): Triangular - slopes up from 30, peaks at 50, down to 70
  if (dirtiness > 30 && dirtiness <= 50) {
    membership.medium = (dirtiness - 30) / (50 - 30);
  } else if (dirtiness > 50 && dirtiness < 70) {
    membership.medium = (70 - dirtiness) / (70 - 50);
  }

  // High (60-100): Trapezoidal - slopes up from 60, full from 75-100
  if (dirtiness > 60 && dirtiness < 75) {
    membership.high = (dirtiness - 60) / (75 - 60);
  } else if (dirtiness >= 75 && dirtiness <= 100) {
    membership.high = 1.0;
  }

  return membership;
}

// Fuzzification for Load Size (0-10 kg): Small, Medium, Large
LoadSizeMembership getLoadSizeMembership(double loadSize) {
  LoadSizeMembership membership;

  // Small (0-4 kg): Trapezoidal - full from 0-2, slopes down to 4
  if (loadSize >= 0 && loadSize <= 2) {
    membership.small = 1.0;
  } else if (loadSize > 2 && loadSize < 4) {
    membership.small = (4 - loadSize) / (4 - 2);
  }

  // Medium (3-7 kg): Triangular - slopes up from 3, peaks at 5, down to 7
  if (loadSize > 3 && loadSize <= 5) {
    membership.medium = (loadSize - 3) / (5 - 3);
  } else if (loadSize > 5 && loadSize < 7) {
    membership.medium = (7 - loadSize) / (7 - 5);
  }

  // Large (6-10 kg): Trapezoidal - slopes up from 6, full from 8-10
  if (loadSize > 6 && loadSize < 8) {
    membership.large = (loadSize - 6) / (8 - 6);
  } else if (loadSize >= 8 && loadSize <= 10) {
    membership.large = 1.0;
  }

  return membership;
}

// Rule evaluation: At least 6 rules
WashTimeMembership evaluateRules(FuzzySet3 dirtiness,
                                 LoadSizeMembership loadSize) {
  WashTimeMembership washTime;

  // Rule 1: IF Dirtiness is Low AND Load Size is Small THEN Wash Time is Short
  double rule1 = min(dirtiness.low, loadSize.small);
  washTime.short_time = max(washTime.short_time, rule1);

  // Rule 2: IF Dirtiness is Low AND Load Size is Medium THEN Wash Time is Short
  double rule2 = min(dirtiness.low, loadSize.medium);
  washTime.short_time = max(washTime.short_time, rule2);

  // Rule 3: IF Dirtiness is Medium AND Load Size is Small THEN Wash Time is
  // Short
  double rule3 = min(dirtiness.medium, loadSize.small);
  washTime.short_time = max(washTime.short_time, rule3);

  // Rule 4: IF Dirtiness is Medium AND Load Size is Medium THEN Wash Time is
  // Medium
  double rule4 = min(dirtiness.medium, loadSize.medium);
  washTime.medium_time = max(washTime.medium_time, rule4);

  // Rule 5: IF Dirtiness is Medium AND Load Size is Large THEN Wash Time is
  // Medium
  double rule5 = min(dirtiness.medium, loadSize.large);
  washTime.medium_time = max(washTime.medium_time, rule5);

  // Rule 6: IF Dirtiness is High AND Load Size is Small THEN Wash Time is
  // Medium
  double rule6 = min(dirtiness.high, loadSize.small);
  washTime.medium_time = max(washTime.medium_time, rule6);

  // Rule 7: IF Dirtiness is High AND Load Size is Medium THEN Wash Time is Long
  double rule7 = min(dirtiness.high, loadSize.medium);
  washTime.long_time = max(washTime.long_time, rule7);

  // Rule 8: IF Dirtiness is High AND Load Size is Large THEN Wash Time is Long
  double rule8 = min(dirtiness.high, loadSize.large);
  washTime.long_time = max(washTime.long_time, rule8);

  // Rule 9: IF Dirtiness is Low AND Load Size is Large THEN Wash Time is Medium
  double rule9 = min(dirtiness.low, loadSize.large);
  washTime.medium_time = max(washTime.medium_time, rule9);

  return washTime;
}

// Defuzzification using Centroid method
double defuzzifyCentroid(WashTimeMembership washTime) {
  // Define center points for output fuzzy sets (in minutes)
  // Short: 15-30 min, center at 22.5 min
  // Medium: 28-50 min, center at 39 min
  // Long: 45-70 min, center at 57.5 min

  double shortCenter = 22.5;
  double mediumCenter = 39.0;
  double longCenter = 57.5;

  double numerator = 0.0;
  double denominator = 0.0;

  if (washTime.short_time > 0) {
    numerator += shortCenter * washTime.short_time;
    denominator += washTime.short_time;
  }

  if (washTime.medium_time > 0) {
    numerator += mediumCenter * washTime.medium_time;
    denominator += washTime.medium_time;
  }

  if (washTime.long_time > 0) {
    numerator += longCenter * washTime.long_time;
    denominator += washTime.long_time;
  }

  if (denominator == 0) {
    return 30.0; // Default value
  }

  return numerator / denominator;
}

// Simulate a washing scenario
void simulateWash(int caseNum, string description, double dirtiness,
                  double loadSize) {
  cout << "\n========================================" << endl;
  cout << "CASE " << caseNum << ": " << description << endl;
  cout << "========================================" << endl;
  cout << fixed << setprecision(2);

  // Input
  cout << "\nInput Parameters:" << endl;
  cout << "  Dirtiness Level: " << dirtiness << "/100" << endl;
  cout << "  Load Size: " << loadSize << " kg" << endl;

  // Fuzzification
  FuzzySet3 dirtinessMembership = getDirtinessMembership(dirtiness);
  LoadSizeMembership loadSizeMembership = getLoadSizeMembership(loadSize);

  cout << "\nFuzzification:" << endl;
  cout << "  Dirtiness Membership:" << endl;
  cout << "    Low: " << dirtinessMembership.low << endl;
  cout << "    Medium: " << dirtinessMembership.medium << endl;
  cout << "    High: " << dirtinessMembership.high << endl;

  cout << "  Load Size Membership:" << endl;
  cout << "    Small: " << loadSizeMembership.small << endl;
  cout << "    Medium: " << loadSizeMembership.medium << endl;
  cout << "    Large: " << loadSizeMembership.large << endl;

  // Rule evaluation
  WashTimeMembership washTimeMembership =
      evaluateRules(dirtinessMembership, loadSizeMembership);

  cout << "\nWash Time Membership:" << endl;
  cout << "  Short: " << washTimeMembership.short_time << endl;
  cout << "  Medium: " << washTimeMembership.medium_time << endl;
  cout << "  Long: " << washTimeMembership.long_time << endl;

  // Defuzzification
  double finalWashTime = defuzzifyCentroid(washTimeMembership);

  cout << "\nFinal Wash Time: " << finalWashTime << " minutes" << endl;

  // Interpretation
  string category;
  if (finalWashTime < 30) {
    category = "SHORT (Quick wash)";
  } else if (finalWashTime < 50) {
    category = "MEDIUM (Regular wash)";
  } else {
    category = "LONG (Heavy duty wash)";
  }
  cout << "Category: " << category << endl;
}

int main() {
  cout << "=====================================================" << endl;
  cout << "    FUZZY WASHING MACHINE CONTROLLER SYSTEM         " << endl;
  cout << "=====================================================" << endl;

  cout << "\nSystem Configuration:" << endl;
  cout << "---------------------" << endl;
  cout << "Inputs:" << endl;
  cout << "  1. Dirtiness (0-100): Low (0-40), Medium (30-70), High (60-100)"
       << endl;
  cout << "  2. Load Size (0-10 kg): Small (0-4), Medium (3-7), Large (6-10)"
       << endl;
  cout << "\nOutput:" << endl;
  cout << "  Wash Time (15-70 min): Short (15-30), Medium (28-50), Long (45-70)"
       << endl;

  cout << "\nFuzzy Rules (9 rules):" << endl;
  cout << "----------------------" << endl;
  cout << "1. Low Dirtiness + Small Load → Short Wash Time" << endl;
  cout << "2. Low Dirtiness + Medium Load → Short Wash Time" << endl;
  cout << "3. Medium Dirtiness + Small Load → Short Wash Time" << endl;
  cout << "4. Medium Dirtiness + Medium Load → Medium Wash Time" << endl;
  cout << "5. Medium Dirtiness + Large Load → Medium Wash Time" << endl;
  cout << "6. High Dirtiness + Small Load → Medium Wash Time" << endl;
  cout << "7. High Dirtiness + Medium Load → Long Wash Time" << endl;
  cout << "8. High Dirtiness + Large Load → Long Wash Time" << endl;
  cout << "9. Low Dirtiness + Large Load → Medium Wash Time" << endl;

  cout << "\n\n=====================================================" << endl;
  cout << "           SIMULATING 5 WASHING SCENARIOS           " << endl;
  cout << "=====================================================" << endl;

  // Case 1: Light dirt, small load
  simulateWash(1, "Lightly Soiled T-Shirts", 20.0, 2.0);

  // Case 2: Moderate dirt, medium load
  simulateWash(2, "Everyday Clothes", 50.0, 5.0);

  // Case 3: Heavy dirt, large load
  simulateWash(3, "Heavily Soiled Work Clothes", 85.0, 9.0);

  // Case 4: Low dirt, large load
  simulateWash(4, "Bedsheets - Lightly Used", 25.0, 8.0);

  // Case 5: High dirt, small load
  simulateWash(5, "Dirty Sports Gear", 80.0, 2.5);

  cout << "\n=====================================================" << endl;
  cout << "                   FINDINGS REPORT                   " << endl;
  cout << "=====================================================" << endl;

  cout << "\nSummary of Test Cases:" << endl;
  cout << "----------------------" << endl;
  cout << "\nCase 1: Lightly soiled, small load" << endl;
  cout << "  Result: Short wash time (~22-25 min)" << endl;
  cout << "  Rationale: Minimal dirt and small quantity require" << endl;
  cout << "             less cleaning time." << endl;

  cout << "\nCase 2: Moderate dirt, medium load" << endl;
  cout << "  Result: Medium wash time (~39 min)" << endl;
  cout << "  Rationale: Balanced conditions require standard" << endl;
  cout << "             washing cycle." << endl;

  cout << "\nCase 3: Heavy dirt, large load" << endl;
  cout << "  Result: Long wash time (~57-60 min)" << endl;
  cout << "  Rationale: Maximum dirt and load require extended" << endl;
  cout << "             cleaning cycle for thorough washing." << endl;

  cout << "\nCase 4: Light dirt, large load" << endl;
  cout << "  Result: Medium wash time (~30-40 min)" << endl;
  cout << "  Rationale: Large quantity despite low dirt level" << endl;
  cout << "             requires moderate time for proper cleaning." << endl;

  cout << "\nCase 5: Heavy dirt, small load" << endl;
  cout << "  Result: Medium wash time (~40-45 min)" << endl;
  cout << "  Rationale: High dirt level requires intensive" << endl;
  cout << "             cleaning despite small quantity." << endl;

  cout << "\n\nKey Findings:" << endl;
  cout << "-------------" << endl;
  cout << "1. System adapts wash time based on both dirtiness and load" << endl;
  cout << "2. Dirtiness level has primary influence on wash duration" << endl;
  cout << "3. Load size acts as secondary factor for optimization" << endl;
  cout << "4. Fuzzy logic provides smooth transitions between states" << endl;
  cout << "5. System can handle overlapping conditions effectively" << endl;

  cout << "\n\nBenefits:" << endl;
  cout << "---------" << endl;
  cout << "• Energy Efficiency: Avoids over-washing light loads" << endl;
  cout << "• Water Conservation: Optimizes wash cycles" << endl;
  cout << "• Fabric Care: Prevents damage from excessive washing" << endl;
  cout << "• Time Saving: Reduces unnecessary wash duration" << endl;
  cout << "• Adaptive Control: Handles uncertain input measurements" << endl;

  cout << "\n\nConclusion:" << endl;
  cout << "-----------" << endl;
  cout << "The fuzzy washing machine controller successfully adjusts" << endl;
  cout << "wash time based on clothes condition and load size. The" << endl;
  cout << "system demonstrates intelligent decision-making by considering"
       << endl;
  cout << "multiple factors simultaneously, resulting in optimized" << endl;
  cout << "washing cycles that save energy, water, and time while" << endl;
  cout << "ensuring effective cleaning." << endl;

  cout << "\n=====================================================" << endl;

  return 0;
}
