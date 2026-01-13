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

struct WaitingTimeMembership {
  double short_time;
  double medium_time;
  double long_time;
  WaitingTimeMembership() : short_time(0.0), medium_time(0.0), long_time(0.0) {}
};

struct GreenLightMembership {
  double short_duration;
  double medium_duration;
  double long_duration;
  GreenLightMembership()
      : short_duration(0.0), medium_duration(0.0), long_duration(0.0) {}
};

// Fuzzification for Vehicle Density (0-100 vehicles): Low, Medium, High
FuzzySet3 getVehicleDensityMembership(double density) {
  FuzzySet3 membership;

  // Low (0-40): Trapezoidal - full from 0-25, slopes down to 40
  if (density >= 0 && density <= 25) {
    membership.low = 1.0;
  } else if (density > 25 && density < 40) {
    membership.low = (40 - density) / (40 - 25);
  }

  // Medium (30-70): Triangular - slopes up from 30, peaks at 50, down to 70
  if (density > 30 && density <= 50) {
    membership.medium = (density - 30) / (50 - 30);
  } else if (density > 50 && density < 70) {
    membership.medium = (70 - density) / (70 - 50);
  }

  // High (60-100): Trapezoidal - slopes up from 60, full from 75-100
  if (density > 60 && density < 75) {
    membership.high = (density - 60) / (75 - 60);
  } else if (density >= 75 && density <= 100) {
    membership.high = 1.0;
  }

  return membership;
}

// Fuzzification for Waiting Time (0-120 seconds): Short, Medium, Long
WaitingTimeMembership getWaitingTimeMembership(double waitTime) {
  WaitingTimeMembership membership;

  // Short (0-40s): Trapezoidal - full from 0-20, slopes down to 40
  if (waitTime >= 0 && waitTime <= 20) {
    membership.short_time = 1.0;
  } else if (waitTime > 20 && waitTime < 40) {
    membership.short_time = (40 - waitTime) / (40 - 20);
  }

  // Medium (30-80s): Triangular - slopes up from 30, peaks at 55, down to 80
  if (waitTime > 30 && waitTime <= 55) {
    membership.medium_time = (waitTime - 30) / (55 - 30);
  } else if (waitTime > 55 && waitTime < 80) {
    membership.medium_time = (80 - waitTime) / (80 - 55);
  }

  // Long (70-120s): Trapezoidal - slopes up from 70, full from 90-120
  if (waitTime > 70 && waitTime < 90) {
    membership.long_time = (waitTime - 70) / (90 - 70);
  } else if (waitTime >= 90 && waitTime <= 120) {
    membership.long_time = 1.0;
  }

  return membership;
}

// Rule evaluation: At least 6 rules
GreenLightMembership evaluateRules(FuzzySet3 density,
                                   WaitingTimeMembership waitTime) {
  GreenLightMembership greenLight;

  // Rule 1: IF Density is Low AND Waiting Time is Short THEN Green Light
  // Duration is Short
  double rule1 = min(density.low, waitTime.short_time);
  greenLight.short_duration = max(greenLight.short_duration, rule1);

  // Rule 2: IF Density is Low AND Waiting Time is Medium THEN Green Light
  // Duration is Short
  double rule2 = min(density.low, waitTime.medium_time);
  greenLight.short_duration = max(greenLight.short_duration, rule2);

  // Rule 3: IF Density is Medium AND Waiting Time is Short THEN Green Light
  // Duration is Short
  double rule3 = min(density.medium, waitTime.short_time);
  greenLight.short_duration = max(greenLight.short_duration, rule3);

  // Rule 4: IF Density is Medium AND Waiting Time is Medium THEN Green Light
  // Duration is Medium
  double rule4 = min(density.medium, waitTime.medium_time);
  greenLight.medium_duration = max(greenLight.medium_duration, rule4);

  // Rule 5: IF Density is Medium AND Waiting Time is Long THEN Green Light
  // Duration is Medium
  double rule5 = min(density.medium, waitTime.long_time);
  greenLight.medium_duration = max(greenLight.medium_duration, rule5);

  // Rule 6: IF Density is High AND Waiting Time is Short THEN Green Light
  // Duration is Medium
  double rule6 = min(density.high, waitTime.short_time);
  greenLight.medium_duration = max(greenLight.medium_duration, rule6);

  // Rule 7: IF Density is High AND Waiting Time is Medium THEN Green Light
  // Duration is Long
  double rule7 = min(density.high, waitTime.medium_time);
  greenLight.long_duration = max(greenLight.long_duration, rule7);

  // Rule 8: IF Density is High AND Waiting Time is Long THEN Green Light
  // Duration is Long
  double rule8 = min(density.high, waitTime.long_time);
  greenLight.long_duration = max(greenLight.long_duration, rule8);

  // Rule 9: IF Density is Low AND Waiting Time is Long THEN Green Light
  // Duration is Medium
  double rule9 = min(density.low, waitTime.long_time);
  greenLight.medium_duration = max(greenLight.medium_duration, rule9);

  return greenLight;
}

// Defuzzification using Centroid method
double defuzzifyCentroid(GreenLightMembership greenLight) {
  // Define center points for output fuzzy sets (in seconds)
  // Short: 15-35s, center at 25s
  // Medium: 30-60s, center at 45s
  // Long: 55-90s, center at 70s

  double shortCenter = 25.0;
  double mediumCenter = 45.0;
  double longCenter = 70.0;

  double numerator = 0.0;
  double denominator = 0.0;

  if (greenLight.short_duration > 0) {
    numerator += shortCenter * greenLight.short_duration;
    denominator += greenLight.short_duration;
  }

  if (greenLight.medium_duration > 0) {
    numerator += mediumCenter * greenLight.medium_duration;
    denominator += greenLight.medium_duration;
  }

  if (greenLight.long_duration > 0) {
    numerator += longCenter * greenLight.long_duration;
    denominator += greenLight.long_duration;
  }

  if (denominator == 0) {
    return 30.0; // Default value
  }

  return numerator / denominator;
}

// Simulate a traffic scenario
void simulateTraffic(int scenario, string description, double density,
                     double waitTime) {
  cout << "\n========================================" << endl;
  cout << "SCENARIO " << scenario << ": " << description << endl;
  cout << "========================================" << endl;
  cout << fixed << setprecision(2);

  // Input
  cout << "\nTraffic Conditions:" << endl;
  cout << "  Vehicle Density: " << density << " vehicles" << endl;
  cout << "  Waiting Time: " << waitTime << " seconds" << endl;

  // Fuzzification
  FuzzySet3 densityMembership = getVehicleDensityMembership(density);
  WaitingTimeMembership waitTimeMembership = getWaitingTimeMembership(waitTime);

  cout << "\nFuzzification:" << endl;
  cout << "  Vehicle Density Membership:" << endl;
  cout << "    Low: " << densityMembership.low << endl;
  cout << "    Medium: " << densityMembership.medium << endl;
  cout << "    High: " << densityMembership.high << endl;

  cout << "  Waiting Time Membership:" << endl;
  cout << "    Short: " << waitTimeMembership.short_time << endl;
  cout << "    Medium: " << waitTimeMembership.medium_time << endl;
  cout << "    Long: " << waitTimeMembership.long_time << endl;

  // Rule evaluation
  GreenLightMembership greenLightMembership =
      evaluateRules(densityMembership, waitTimeMembership);

  cout << "\nGreen Light Duration Membership:" << endl;
  cout << "  Short: " << greenLightMembership.short_duration << endl;
  cout << "  Medium: " << greenLightMembership.medium_duration << endl;
  cout << "  Long: " << greenLightMembership.long_duration << endl;

  // Defuzzification
  double greenLightDuration = defuzzifyCentroid(greenLightMembership);

  cout << "\nFinal Green Light Duration: " << greenLightDuration << " seconds"
       << endl;

  // Interpretation
  string category;
  if (greenLightDuration < 35) {
    category = "SHORT (Light traffic)";
  } else if (greenLightDuration < 60) {
    category = "MEDIUM (Moderate traffic)";
  } else {
    category = "LONG (Heavy traffic)";
  }
  cout << "Category: " << category << endl;
}

int main() {
  cout << "=====================================================" << endl;
  cout << "     FUZZY TRAFFIC LIGHT CONTROL SYSTEM             " << endl;
  cout << "=====================================================" << endl;

  cout << "\nSystem Configuration:" << endl;
  cout << "---------------------" << endl;
  cout << "Inputs:" << endl;
  cout << "  1. Vehicle Density (0-100): Low (0-40), Medium (30-70), High "
          "(60-100)"
       << endl;
  cout << "  2. Waiting Time (0-120s): Short (0-40s), Medium (30-80s), Long "
          "(70-120s)"
       << endl;
  cout << "\nOutput:" << endl;
  cout << "  Green Light Duration (15-90s): Short (15-35s), Medium (30-60s), "
          "Long (55-90s)"
       << endl;

  cout << "\nFuzzy Rules (9 rules):" << endl;
  cout << "----------------------" << endl;
  cout << "1. Low Density + Short Wait → Short Duration" << endl;
  cout << "2. Low Density + Medium Wait → Short Duration" << endl;
  cout << "3. Medium Density + Short Wait → Short Duration" << endl;
  cout << "4. Medium Density + Medium Wait → Medium Duration" << endl;
  cout << "5. Medium Density + Long Wait → Medium Duration" << endl;
  cout << "6. High Density + Short Wait → Medium Duration" << endl;
  cout << "7. High Density + Medium Wait → Long Duration" << endl;
  cout << "8. High Density + Long Wait → Long Duration" << endl;
  cout << "9. Low Density + Long Wait → Medium Duration" << endl;

  cout << "\n\n=====================================================" << endl;
  cout << "         SIMULATING TRAFFIC SCENARIOS               " << endl;
  cout << "=====================================================" << endl;

  // Scenario 1: Light traffic, short wait
  simulateTraffic(1, "Early Morning - Light Traffic", 15.0, 10.0);

  // Scenario 2: Moderate traffic, medium wait
  simulateTraffic(2, "Mid-Day - Moderate Traffic", 50.0, 55.0);

  // Scenario 3: Heavy traffic, long wait
  simulateTraffic(3, "Rush Hour - Heavy Traffic", 85.0, 100.0);

  // Scenario 4: High density but short wait (just started)
  simulateTraffic(4, "Heavy Traffic - Just Turned Red", 80.0, 25.0);

  // Scenario 5: Low density but long wait
  simulateTraffic(5, "Late Night - Long Wait", 20.0, 95.0);

  // Scenario 6: Medium density, short wait
  simulateTraffic(6, "Normal Conditions", 45.0, 35.0);

  cout << "\n=====================================================" << endl;
  cout << "                  SIMULATION REPORT                  " << endl;
  cout << "=====================================================" << endl;
  cout << "\nThe fuzzy traffic light control system successfully" << endl;
  cout << "simulated 6 different traffic scenarios." << endl;
  cout << "\nKey Observations:" << endl;
  cout << "- Light traffic with short waiting time results in" << endl;
  cout << "  short green light duration (~25s)." << endl;
  cout << "- Heavy traffic with long waiting time results in" << endl;
  cout << "  extended green light duration (~70s)." << endl;
  cout << "- The system adapts dynamically based on both vehicle" << endl;
  cout << "  density and waiting time." << endl;
  cout << "- Moderate conditions receive balanced green light" << endl;
  cout << "  duration (~45s)." << endl;
  cout << "\nBenefits of Fuzzy Logic:" << endl;
  cout << "- Smooth transitions between different traffic states" << endl;
  cout << "- Handles uncertainty in traffic measurements" << endl;
  cout << "- Considers multiple factors simultaneously" << endl;
  cout << "- Provides adaptive traffic control" << endl;
  cout << "\nConclusion:" << endl;
  cout << "This fuzzy system optimizes traffic flow by adjusting" << endl;
  cout << "green light duration based on real-time conditions," << endl;
  cout << "reducing wait times and improving traffic efficiency." << endl;
  cout << "=====================================================" << endl;

  return 0;
}
