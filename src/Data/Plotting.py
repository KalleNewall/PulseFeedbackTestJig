import matplotlib.pyplot as plt

# File paths
input_file = "src\Data\data-20241202-123122"
output_file = "cleaned_data.csv"

# Clean the input file
with open(input_file, "r", encoding="utf-8", errors="ignore") as infile, open(output_file, "w", encoding="utf-8") as outfile:
    for line in infile:
        if line.startswith("=") or "PuTTY log" in line or line.strip() == "":
            continue
        try:
            outfile.write(line.strip() + "\n")
        except UnicodeDecodeError:
            print(f"Skipped a malformed line: {line}")

print(f"Cleaned data saved to {output_file}")

# Initialize data containers
potentiometer_values = []
scale_values = []
time_values = []

# Read and process the cleaned data file
with open(output_file, "r", encoding="utf-8") as datafile:
    data_lines = [line.strip() for line in datafile if line.strip()]

    # Each set of three lines corresponds to one data point
    for i in range(0, len(data_lines), 3):
        try:
            potentiometer_values.append(int(data_lines[i]))  # First value: Potentiometer
            scale_values.append(float(data_lines[i + 1]))    # Second value: Load cell
            time_values.append(int(data_lines[i + 2]))       # Third value: Time in millis
        except (ValueError, IndexError):
            print(f"Skipped malformed data point at line {i}: {data_lines[i:i+3]}")

# Normalize time values to start at 0
start_time = time_values[0]
time_values = [t - start_time for t in time_values]

# Conversion factor from ADC to mm
adc_to_mm_factor = 0.01035  # mm per ADC step

#adc_to_mm_factor = 0.0659 # degrees per ADC step

# Conversion factor from grams to Newtons
grams_to_newtons_factor = 0.00980665  # Newtons per gram

# Convert potentiometer values from ADC to millimeters
potentiometer_mm_values = [adc_value * adc_to_mm_factor for adc_value in potentiometer_values]

# Convert scale values from grams to Newtons
scale_newton_values = [scale_value * grams_to_newtons_factor for scale_value in scale_values]

# Plotting the data
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8))

# Plot potentiometer values (mm) against normalized time (ms)
ax1.plot(time_values, potentiometer_mm_values, label="Pulse Amplitude [mm]", linestyle="-", marker="o")
ax1.set_title("Pulse Position Over Time")
ax1.set_xlabel("Time [ms]")
ax1.set_ylabel("Pulse Amplitude [mm]")
ax1.legend()
ax1.grid(True)

# Plot scale values (Newtons) against normalized time (ms)
ax2.plot(time_values, scale_newton_values, label="Force [N]", linestyle="--", marker="x", color="orange")
ax2.set_title("Pulse Force Over Time")
ax2.set_xlabel("Time [ms]")
ax2.set_ylabel("Force [N]")
ax2.legend()
ax2.grid(True)

# Adjust layout and display the plot
plt.tight_layout()
plt.show()
