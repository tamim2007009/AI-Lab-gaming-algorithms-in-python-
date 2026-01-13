#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

using namespace std;

// Structure to hold membership values
struct Membership {
  double low;
  double medium;
  double high;

  Membership() : low(0.0), medium(0.0), high(0.0) {}
};

struct HumidityMembership {
  double dry;
  double comfortable;
  double humid;

  HumidityMembership() : dry(0.0), comfortable(0.0), humid(0.0) {}
};

struct FanSpeedMembership {
  double slow;
  double medium;
  double fast;

  FanSpeedMembership() : slow(0.0), medium(0.0), fast(0.0) {}
};

// Function to calculate temperature membership (Low, Medium, High)
Membership getTemperatureMembership(double temp) {
  Membership membership;

  // Low (0-15°C): Trapezoidal - full membership from 0-10, slopes down to 15
  if (temp >= 0 && temp <= 10) {
    membership.low = 1.0;
  } else if (temp > 10 && temp < 15) {
    membership.low = (15 - temp) / (15 - 10);
  }

  // Medium (15-30°C): Triangular - slopes up from 10, peaks at 22.5, slopes
  // down to 30
  if (temp > 10 && temp <= 22.5) {
    membership.medium = (temp - 10) / (22.5 - 10);
  } else if (temp > 22.5 && temp < 30) {
    membership.medium = (30 - temp) / (30 - 22.5);
  }

  // High (30-45°C): Trapezoidal - slopes up from 25, full membership from 30-45
  if (temp > 25 && temp < 30) {
    membership.high = (temp - 25) / (30 - 25);
  } else if (temp >= 30 && temp <= 45) {
    membership.high = 1.0;
  }

  return membership;
}

// Function to calculate humidity membership (Dry, Comfortable, Humid)
HumidityMembership getHumidityMembership(double humidity) {
  HumidityMembership membership;

  // Dry (0-30%): Trapezoidal - full membership from 0-20, slopes down to 30
  if (humidity >= 0 && humidity <= 20) {
    membership.dry = 1.0;
  } else if (humidity > 20 && humidity < 30) {
    membership.dry = (30 - humidity) / (30 - 20);
  }

  // Comfortable (30-60%): Triangular - slopes up from 25, peaks at 45, slopes
  // down to 60
  if (humidity > 25 && humidity <= 45) {
    membership.comfortable = (humidity - 25) / (45 - 25);
  } else if (humidity > 45 && humidity < 60) {
    membership.comfortable = (60 - humidity) / (60 - 45);
  }

  // Humid (60-100%): Trapezoidal - slopes up from 55, full membership from
  // 60-100
  if (humidity > 55 && humidity < 60) {
    membership.humid = (humidity - 55) / (60 - 55);
  } else if (humidity >= 60 && humidity <= 100) {
    membership.humid = 1.0;
  }

  return membership;
}

// Function to evaluate fuzzy rules and return fan speed membership
FanSpeedMembership evaluateRules(Membership temp, HumidityMembership hum) {
  FanSpeedMembership fanSpeed;

  // Rule 1: IF Temperature is Low AND Humidity is Dry THEN Fan Speed is Slow
  double rule1 = min(temp.low, hum.dry);
  fanSpeed.slow = max(fanSpeed.slow, rule1);

  // Rule 2: IF Temperature is Low AND Humidity is Comfortable THEN Fan Speed is
  // Slow
  double rule2 = min(temp.low, hum.comfortable);
  fanSpeed.slow = max(fanSpeed.slow, rule2);

  // Rule 3: IF Temperature is Low AND Humidity is Humid THEN Fan Speed is
  // Medium
  double rule3 = min(temp.low, hum.humid);
  fanSpeed.medium = max(fanSpeed.medium, rule3);

  // Rule 4: IF Temperature is Medium AND Humidity is Dry THEN Fan Speed is Slow
  double rule4 = min(temp.medium, hum.dry);
  fanSpeed.slow = max(fanSpeed.slow, rule4);

  // Rule 5: IF Temperature is Medium AND Humidity is Comfortable THEN Fan Speed
  // is Medium
  double rule5 = min(temp.medium, hum.comfortable);
  fanSpeed.medium = max(fanSpeed.medium, rule5);

  // Rule 6: IF Temperature is Medium AND Humidity is Humid THEN Fan Speed is
  // Fast
  double rule6 = min(temp.medium, hum.humid);
  fanSpeed.fast = max(fanSpeed.fast, rule6);

  // Rule 7: IF Temperature is High AND Humidity is Dry THEN Fan Speed is Medium
  double rule7 = min(temp.high, hum.dry);
  fanSpeed.medium = max(fanSpeed.medium, rule7);

  // Rule 8: IF Temperature is High AND Humidity is Comfortable THEN Fan Speed
  // is Fast
  double rule8 = min(temp.high, hum.comfortable);
  fanSpeed.fast = max(fanSpeed.fast, rule8);

  // Rule 9: IF Temperature is High AND Humidity is Humid THEN Fan Speed is Fast
  double rule9 = min(temp.high, hum.humid);
  fanSpeed.fast = max(fanSpeed.fast, rule9);

  return fanSpeed;
}

// Defuzzification using Centroid Method
double defuzzifyCentroid(FanSpeedMembership fanSpeed) {
  // Define center points for each output fuzzy set
  // Slow: 0-40, center at 20
  // Medium: 40-70, center at 55
  // Fast: 70-100, center at 85

  double slowCenter = 20.0;
  double mediumCenter = 55.0;
  double fastCenter = 85.0;

  double numerator = 0.0;
  double denominator = 0.0;

  // Calculate weighted sum
  if (fanSpeed.slow > 0) {
    numerator += slowCenter * fanSpeed.slow;
    denominator += fanSpeed.slow;
  }

  if (fanSpeed.medium > 0) {
    numerator += mediumCenter * fanSpeed.medium;
    denominator += fanSpeed.medium;
  }

  if (fanSpeed.fast > 0) {
    numerator += fastCenter * fanSpeed.fast;
    denominator += fanSpeed.fast;
  }

  if (denominator == 0) {
    return 0.0; // Default value if no rules fire
  }

  return numerator / denominator;
}

// Function to test a single case
void testCase(int caseNum, double temperature, double humidity) {
  cout << "\n========================================" << endl;
  cout << "TEST CASE " << caseNum << endl;
  cout << "========================================" << endl;
  cout << fixed << setprecision(2);

  // Input
  cout << "\nCrisp Inputs:" << endl;
  cout << "  Temperature: " << temperature << "°C" << endl;
  cout << "  Humidity: " << humidity << "%" << endl;

  // Fuzzification
  Membership tempMembership = getTemperatureMembership(temperature);
  HumidityMembership humMembership = getHumidityMembership(humidity);

  cout << "\nFuzzification:" << endl;
  cout << "  Temperature Membership:" << endl;
  cout << "    Low: " << tempMembership.low << endl;
  cout << "    Medium: " << tempMembership.medium << endl;
  cout << "    High: " << tempMembership.high << endl;

  cout << "  Humidity Membership:" << endl;
  cout << "    Dry: " << humMembership.dry << endl;
  cout << "    Comfortable: " << humMembership.comfortable << endl;
  cout << "    Humid: " << humMembership.humid << endl;

  // Rule Evaluation
  FanSpeedMembership fanSpeedMembership =
      evaluateRules(tempMembership, humMembership);

  cout << "\nRule Output Strengths:" << endl;
  cout << "  Fan Speed Membership:" << endl;
  cout << "    Slow: " << fanSpeedMembership.slow << endl;
  cout << "    Medium: " << fanSpeedMembership.medium << endl;
  cout << "    Fast: " << fanSpeedMembership.fast << endl;

  // Defuzzification
  double finalFanSpeed = defuzzifyCentroid(fanSpeedMembership);

  cout << "\nDefuzzification:" << endl;
  cout << "  Final Fan Speed: " << finalFanSpeed << " (0-100 scale)" << endl;

  // Interpretation
  string speedLabel;
  if (finalFanSpeed < 40) {
    speedLabel = "SLOW";
  } else if (finalFanSpeed < 70) {
    speedLabel = "MEDIUM";
  } else {
    speedLabel = "FAST";
  }
  cout << "  Interpretation: " << speedLabel << endl;
}

int main() {
  cout << "=================================================" << endl;
  cout << "   FUZZY LOGIC-BASED ROOM TEMPERATURE CONTROL   " << endl;
  cout << "         Fan/AC Speed Control System            " << endl;
  cout << "=================================================" << endl;

  cout << "\nSystem Configuration:" << endl;
  cout << "---------------------" << endl;
  cout << "Input Variables:" << endl;
  cout << "  1. Temperature: Low (0-15°C), Medium (15-30°C), High (30-45°C)"
       << endl;
  cout << "  2. Humidity: Dry (0-30%), Comfortable (30-60%), Humid (60-100%)"
       << endl;
  cout << "\nOutput Variable:" << endl;
  cout << "  Fan Speed: Slow (0-40), Medium (40-70), Fast (70-100)" << endl;

  cout << "\nFuzzy Rules:" << endl;
  cout << "------------" << endl;
  cout << "1. IF Temp=Low AND Humidity=Dry THEN FanSpeed=Slow" << endl;
  cout << "2. IF Temp=Low AND Humidity=Comfortable THEN FanSpeed=Slow" << endl;
  cout << "3. IF Temp=Low AND Humidity=Humid THEN FanSpeed=Medium" << endl;
  cout << "4. IF Temp=Medium AND Humidity=Dry THEN FanSpeed=Slow" << endl;
  cout << "5. IF Temp=Medium AND Humidity=Comfortable THEN FanSpeed=Medium"
       << endl;
  cout << "6. IF Temp=Medium AND Humidity=Humid THEN FanSpeed=Fast" << endl;
  cout << "7. IF Temp=High AND Humidity=Dry THEN FanSpeed=Medium" << endl;
  cout << "8. IF Temp=High AND Humidity=Comfortable THEN FanSpeed=Fast" << endl;
  cout << "9. IF Temp=High AND Humidity=Humid THEN FanSpeed=Fast" << endl;

  // Test Cases
  cout << "\n\n=================================================" << endl;
  cout << "              TESTING WITH 5 CASES              " << endl;
  cout << "=================================================" << endl;

  // Test Case 1: Low temperature, Low humidity
  testCase(1, 8.0, 15.0);

  // Test Case 2: Medium temperature, Comfortable humidity
  testCase(2, 22.0, 45.0);

  // Test Case 3: High temperature, Humid
  testCase(3, 35.0, 75.0);

  // Test Case 4: Medium temperature, High humidity
  testCase(4, 25.0, 65.0);

  // Test Case 5: High temperature, Low humidity
  testCase(5, 32.0, 25.0);

  cout << "\n=================================================" << endl;
  cout << "            TESTING COMPLETE                     " << endl;
  cout << "=================================================" << endl;

  return 0;
}
