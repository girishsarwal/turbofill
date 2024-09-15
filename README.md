# Turbo Fill

_Made for Remote-Controlled Airplanes but use in any filling application_

This Arduino-based system automates the process of filling precise metered doses of fuel for remote-controlled glow planes, gas planes, and jets. The system allows users to input a specific fuel volume, and it controls a pump to dispense that amount accurately. It features a calibration mode for ensuring the pump dispenses the correct volume every time, even if different pumps are used.

# Features
- Keypad Interface: Easily input the desired fuel volume using a 4x3 keypad.
- LCD Display: A 16x2 LiquidCrystal display shows current status, input volume, and operation progress.
- Pump Control: Automatically manages the pump to deliver the exact fuel amount.
- Flow Sensor: Monitors fuel flow and counts pulses to calculate the dispensed volume.
- Calibration Mode: Users can calibrate their pumps to ensure accurate fuel delivery. Once calibrated, the system can dispense fuel based on the specific flow rate of the pump.
- EEPROM Storage: Calibration settings are stored so they remain accurate even after powering off.

## Designed for Remote-Controlled Airplanes:
This system is tailored for hobbyists and enthusiasts who operate remote-controlled planes. Whether you're flying glow planes, gas planes, or jets, the Turbo Fill System ensures precise fuel dispensing, reducing waste and ensuring optimal performance for your aircraft.

# Components Used
- Arduino: Manages the control and logic.
- LiquidCrystal Display (LCD): Provides feedback and shows the operational status.
- Keypad: Used for entering fuel volume in milliliters.
- Pump: Controlled via a digital pin to pump the fuel.
- Flow Sensor: Detects the amount of fuel being pumped by counting pulses. Any analog flow sensor should work
- EEPROM: Saves calibration settings for future use.

# Wiring Connections
- LCD: Connected to Arduino pins (8, 3, 4, 5, 6, 7).
- Keypad: Rows connected to analog pins (A0, A1, A2, A3) and columns to digital pins (9, 10, 11).
- Pump: Connected to pin 12 (for pump control).
- Flow Sensor: Connected to pin 2 (for pulse counting).
- Start Button: Connected to pin 13 (for starting fuel filling).

# Keypad Controls
- 0-9: Input the desired fuel volume in milliliters.
- *: Enter calibration mode or pause during fuel filling.
- #: Reset the current input volume.
- Menu will show isntructions always

# Operation Modes
## Main Mode

- Input the required fuel volume (0-9999 ml).
- Press the start button (pin 13) to begin filling the fuel tank.
- The pump will automatically stop once the desired volume is reached.

## Filling Mode
- Displays the current filled fuel volume.
- Allows users to pause or cancel the operation at any time.

## Calibration Mode
- Specify a target volume for calibration.
- The pump will start filling the target volume
- Stop when the target amount is reached.
- Save the calibration data to EEPROM for future use, ensuring pump accuracy
- Pump will now only fill the set amount based on this calibrated amount
  
# Calibration:
This system's calibration mode is key for precision. Whether you're using different pumps or fuel types, calibration ensures accurate measurements by allowing you to adjust for your pump’s flow rate. Once calibrated, the system will consistently pump the exact amount of fuel required.

# Circuit Diagram:
To assemble this system, follow the wiring configuration:

[COMING SOON]

Arduino pins are connected to the LCD, keypad, flow sensor, and pump as described above.
Ensure that the flow sensor and pump are properly grounded and connected to their respective power sources.

# Setup and Use:
### Initial Calibration 
If the system is being used for the first time or with a new pump, run the calibration mode to set accurate flow rates.

###Fuel Filling 
Once calibrated, simply input the desired fuel volume and let the system handle the rest.

# Future Enhancements:
Add error detection for fuel pump malfunctions.
Implement low-fuel and overfill warnings for added safety.
This system streamlines fuel filling for remote-controlled planes, making it easier and more efficient to prepare for flight with precision.


# Licensing
This project is licensed under the MIT License. Feel free to use, modify, and distribute the code as long as the original author is credited. See the LICENSE file for more details.

# Contribution
Contributions are welcome! If you'd like to improve the system or add new features, feel free to open a pull request or issue on the GitHub repository. Please follow the standard contribution guidelines:

- Fork the repository.
- Create a new branch for your feature (git checkout -b feature-name).
- Commit your changes (git commit -m 'Add feature').
- Push to the branch (git push origin feature-name).
- Open a pull request for review.
= Improvements and suggestions to enhance fuel precision, pump error detection, or user interface are encouraged.
