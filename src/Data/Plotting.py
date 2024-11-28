import matplotlib.pyplot as plt
import csv

input_file = "src\Data\data-20241128-122631"  
output_file = "cleaned_data.csv"

with open(input_file, "r") as infile, open(output_file, "w") as outfile:
    for line in infile:
        if line.startswith("=") or "PuTTY log" in line:
            continue
        
        outfile.write(line)

print(f"Cleaned data saved to {output_file}")

scale_values = []
potentiometer_values = []

with open(output_file, "r") as csvfile:
    reader = csv.reader(csvfile, delimiter=':')
    for row in reader:
        if len(row) == 2:  # Ensure row has a label and value
            key, value = row[0].strip(), row[1].strip()
            if key == "Scale_value":
                scale_values.append(float(value))
            elif key == "Potentiometer_value":
                potentiometer_values.append(int(value))

NUM_POINTS_TO_SKIP = 5
scale_values = scale_values[NUM_POINTS_TO_SKIP:]
potentiometer_values = potentiometer_values[NUM_POINTS_TO_SKIP:]

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 10))  # Two rows, one column

ax1.plot(scale_values, label="Scale Value", linestyle="--", marker="o")
ax1.set_title("Scale Values Over Time")
ax1.set_xlabel("Sample Index")
ax1.set_ylabel("Scale Value")
ax1.legend()
ax1.grid(True)

ax2.plot(potentiometer_values, label="Potentiometer Value", linestyle="-", marker="x", color="orange")
ax2.set_title("Potentiometer Values Over Time")
ax2.set_xlabel("Sample Index")
ax2.set_ylabel("Potentiometer Value")
ax2.legend()
ax2.grid(True)


plt.tight_layout()
plt.show()