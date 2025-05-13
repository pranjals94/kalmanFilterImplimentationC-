/*
The code below shows the usage of Kalman filter and low pass filter for smoothing noisy signals 
*/

// Kalman filter variables
float estimate = 0;        // Current estimated value

// If your initial estimate error "error_estimate" is too small, Kalman Gain starts low and takes time to adjust.
float error_estimate = 25;  // Error in estimate (Adjust)

// Lower measurement noise (more trust(responsive) in sensor)
float error_measure = 10;   // Error in measurement (Adjust)

float kalman_gain = 0.0;

float lowpass_output = 0.0;     // Exponential filter output
float alpha = 0.07;              // Low-pass filter strength (0.0 to 1.0)

const int adcPin = A0;

void setup() {
  Serial.begin(19200);
  
}

void loop() {
  mainFunction();
  // while(1);// run the loop only once
 
}


void mainFunction(){
  // Read raw ADC value
  int raw = analogRead(adcPin);
  
  // Convert to float (optional, depending on what you're filtering)
  float measurement = (float)raw;

   // Dynamically boost error if sharp change
  if (abs(measurement - estimate) > 10) {
    error_estimate = 10.0;
  }

  // Kalman gain calculation
  kalman_gain = error_estimate / (error_estimate + error_measure);

  // Update estimate with measurement
  estimate = estimate + kalman_gain * (measurement - estimate);

  // // Update the estimation error
  // error_estimate = (1 - kalman_gain) * error_estimate;

  // Update Error Estimate (decay back down to stabilize over time)
  error_estimate = max(5.0, (1.0 - kalman_gain) * error_estimate);

  // Exponential Low-Pass Filter (final smoothing)
  lowpass_output = alpha * estimate + (1.0 - alpha) * lowpass_output;

  // Print the data series, ensuring each one is separated by a tab
  Serial.print("Raw:");            
  Serial.print(raw);
  Serial.print("\t");              // Tab to separate values

  Serial.print("Filtered:");       
  Serial.println(lowpass_output);

   delay(4); // adjust for faster response

}


/*

* 60 Hz grid noise can be seen whe the laptop is connected to the charger


There will always be some delay between the raw data and the filtered data — and this is normal and unavoidable for any real-time filter.
you may get fast response by using faster processing speed, like esp32 or STM 32 mcus that can run at greater frequency.

🧠 Why There’s Always Some Delay
Filtering = Smoothing + Prediction
A filter (like Kalman or low-pass) tries to remove noise. To do that, it must:

Compare the current input with past values

Weight how much it trusts the new input

React slowly to avoid reacting to noise

This means it can never perfectly follow rapid changes immediately — that's the trade-off between smoothness and speed.

📊 How Much Delay?
Typical Filters:
Filter Type	Delay Characteristics
Raw ADC	No delay (but noisy)
Low-pass filter	Delay ≈ ~(1 - α) × response time
Kalman filter	Delay depends on gain (lower gain = more delay)
Moving average (N)	Delay ≈ N/2 samples
Double Kalman	Delay is cumulative (longer)

So yes — the more stable your filter, the longer it lags behind the real signal.

🔁 Example:
Let’s say you turn the potentiometer quickly:

Time (ms)	Raw ADC	Filtered Output
0	100	100
10	600	200 ← filter starts responding
20	600	300 ← catching up
30	600	400 ← lag persists
40	600	500 ← approaching target

Even with good tuning, the filter takes time to “catch up” — this is normal.

✅ How to Minimize Delay:
Increase Kalman gain or reduce error_measure

Use dynamic Kalman gain (you already are)

Reduce low-pass alpha (closer to 0.1–0.2)

Avoid stacking filters unless truly needed

Boost response on sudden changes (delta > threshold)
*/