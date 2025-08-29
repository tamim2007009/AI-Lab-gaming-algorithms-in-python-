def get_ambient_light_membership(lux):
    """
    Calculates the membership degrees for the 'Ambient Light' input.

    This function defines trapezoidal and triangular membership functions for the fuzzy sets
    'Dark', 'Dim', and 'Bright' based on the lux value.

    Args:
        lux (float): The crisp input value for ambient light in Lux.

    Returns:
        dict: A dictionary containing the membership degrees for 'Dark', 'Dim', and 'Bright'.
              Example: {'Dark': 0.8, 'Dim': 0.2, 'Bright': 0.0}
    """
    membership = {'Dark': 0.0, 'Dim': 0.0, 'Bright': 0.0}

    # Membership for 'Dark' (Trapezoidal: flat top from 0 to 40, slopes down to 50)
    if 0 <= lux <= 40:
        membership['Dark'] = 1.0
    elif 40 < lux < 50:
        membership['Dark'] = (50 - lux) / (50 - 40)

    # Membership for 'Dim' (Triangular: slopes up from 40 to 95, down to 150)
    if 40 < lux <= 95:
        membership['Dim'] = (lux - 40) / (95 - 40)
    elif 95 < lux < 150:
        membership['Dim'] = (150 - lux) / (150 - 95)

    # Membership for 'Bright' (Trapezoidal: slopes up from 100, flat top from 150 to 500)
    if 100 < lux < 150:
        membership['Bright'] = (lux - 100) / (150 - 100)
    elif 150 <= lux <= 500:
        membership['Bright'] = 1.0

    return membership


def get_user_preference_membership(preference):
    """
    Calculates the membership degrees for the 'User Preference' input.

    Args:
        preference (float): The crisp input value for user preference (0-100).

    Returns:
        dict: A dictionary containing the membership degrees for 'Dim', 'Low', 'Medium', 'High', and 'Bright'.
    """
    membership = {'Dim': 0.0, 'Low': 0.0, 'Medium': 0.0, 'High': 0.0, 'Bright': 0.0}

    # Membership for 'Dim' (Triangular)
    if 0 <= preference < 20:
        membership['Dim'] = 1.0
    elif 20 <= preference < 30:
        membership['Dim'] = (30 - preference) / (30 - 20)

    # Membership for 'Low' (Triangular)
    if 20 < preference < 35:
        membership['Low'] = (preference - 20) / (35 - 20)
    elif 35 <= preference < 50:
        membership['Low'] = (50 - preference) / (50 - 35)

    # Membership for 'Medium' (Triangular)
    if 40 < preference < 55:
        membership['Medium'] = (preference - 40) / (55 - 40)
    elif 55 <= preference < 70:
        membership['Medium'] = (70 - preference) / (70 - 55)

    # Membership for 'High' (Triangular)
    if 60 < preference < 75:
        membership['High'] = (preference - 60) / (75 - 60)
    elif 75 <= preference < 90:
        membership['High'] = (90 - preference) / (90 - 75)

    # Membership for 'Bright' (Triangular)
    if 80 < preference < 90:
        membership['Bright'] = (preference - 80) / (90 - 80)
    elif 90 <= preference <= 100:
        membership['Bright'] = 1.0
        
    return membership


def evaluate_rules(ambient_membership, preference_membership):
    """
    Evaluates the fuzzy rules to determine the strength of each output.

    Args:
        ambient_membership (dict): Membership degrees for ambient light.
        preference_membership (dict): Membership degrees for user preference.

    Returns:
        dict: A dictionary of the output strengths for each brightness level.
    """
    strengths = {'Dim': 0.0, 'Low': 0.0, 'Medium': 0.0, 'High': 0.0, 'Bright': 0.0}

    # Rule 1: IF Ambient Light is Dark AND User Preference is Dim THEN Brightness is Dim
    strength_rule1 = min(ambient_membership['Dark'], preference_membership['Dim'])
    strengths['Dim'] = max(strengths['Dim'], strength_rule1)

    # Rule 2: IF Ambient Light is Dark AND User Preference is Low THEN Brightness is Low
    strength_rule2 = min(ambient_membership['Dark'], preference_membership['Low'])
    strengths['Low'] = max(strengths['Low'], strength_rule2)

    # Rule 3: IF Ambient Light is Dim AND User Preference is Low THEN Brightness is Dim
    strength_rule3 = min(ambient_membership['Dim'], preference_membership['Low'])
    strengths['Dim'] = max(strengths['Dim'], strength_rule3)

    # Rule 4: IF Ambient Light is Dim AND User Preference is Medium THEN Brightness is Medium
    strength_rule4 = min(ambient_membership['Dim'], preference_membership['Medium'])
    strengths['Medium'] = max(strengths['Medium'], strength_rule4)

    # Rule 5: IF Ambient Light is Bright AND User Preference is Bright THEN Brightness is Bright
    strength_rule5 = min(ambient_membership['Bright'], preference_membership['Bright'])
    strengths['Bright'] = max(strengths['Bright'], strength_rule5)

    return strengths

def defuzzify_centroid(strengths):
    """
    Defuzzifies the rule strengths into a crisp output using the centroid method.

    This function defines the output fuzzy sets for brightness and calculates the
    center of gravity of the aggregated shape.

    Args:
        strengths (dict): The output strengths for each brightness level.

    Returns:
        float: The final crisp brightness level (0-100). Returns 0 if no rules fired.
    """
    # Define output fuzzy set ranges for brightness levels
    output_sets = {
        'Dim': (0, 10, 20),      # Center at 10
        'Low': (15, 30, 45),     # Center at 30
        'Medium': (40, 55, 70),  # Center at 55
        'High': (65, 80, 95),    # Center at 80
        'Bright': (80, 90, 100)  # Center at 90
    }

    numerator = 0
    denominator = 0

    # Use the center of each output set as a representative point
    for level, strength in strengths.items():
        if strength > 0:
            center_point = output_sets[level][1]  # Use the middle value as the center
            numerator += center_point * strength
            denominator += strength

    if denominator == 0:
        return 0  # Default value if no rules are activated

    return numerator / denominator


# --- Main Execution ---

# 1. Define Crisp Inputs
ambient_light_input = 45  # Example: 45 Lux (falls between Dark and Dim)
user_preference_input = 38 # Example: 38 (falls between Low and Medium)

# 2. Fuzzification
ambient_membership = get_ambient_light_membership(ambient_light_input)
preference_membership = get_user_preference_membership(user_preference_input)

# 3. Rule Evaluation
rule_strengths = evaluate_rules(ambient_membership, preference_membership)

# 4. Defuzzification
final_brightness = defuzzify_centroid(rule_strengths)



print(f"Crisp Inputs:")
print(f"  - Ambient Light: {ambient_light_input} Lux")
print(f"  - User Preference: {user_preference_input}")
print("-" * 30)
print(f"Fuzzified Input Membership:")
print(f"  - Ambient Light: {ambient_membership}")
print(f"  - User Preference: {preference_membership}")
print("-" * 30)
print(f"Rule Output Strengths:")
print(f"  - Brightness Levels: {rule_strengths}")
print("-" * 30)
print(f"Final Crisp Output:")
print(f"  - Calculated Brightness: {final_brightness:.2f}%")