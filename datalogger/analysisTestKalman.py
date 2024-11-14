import numpy as np
from filterpy.kalman import KalmanFilter
import matplotlib.pyplot as plt

import pandas as pd

threshold = 0.
maxChange = 1

data = pd.read_csv('serial_log.csv',sep=';')

t =[]
tsum = 0
for i in range(len(data['dt'])):
    t.append(tsum)
    tsum+=data['dt'][i]*10**-3

plt.plot(t,data['aksX'])
plt.show()
# Load data from a CSV file
print(data)
# Example preprocessing steps
data.fillna(method='ffill', inplace=True)  # Fill missing values
# data = data[(data['aksX'] < threshold) & (data['aksY'] < threshold) & (data['aksZ'] < threshold)]  # Filter anomalies

data['aksX'] = (abs(data['aksX'])>threshold) *data['aksX'] 

plt.plot(t,data['aksX'])
plt.show()

# Initialize Kalman Filter for each axis
kf_x = KalmanFilter(dim_x=2, dim_z=1)
kf_x.F = np.array([[1., 1.], [0., 1.]])  # State transition matrix
kf_x.H = np.array([[1., 0.]])            # Measurement matrix
kf_x.P *= 1000.                           # Covariance matrix
kf_x.R = 5                                # Measurement noise
kf_x.Q = 0.1                            # Process noise

# Apply Kalman filter to 'x' axis data
filtered_x = []
for x in data['aksX']:
    kf_x.predict()
    kf_x.update(x)
    filtered_x.append(kf_x.x[0])

data['x_kalman'] = filtered_x

v = []
vprev = 0
for i in range(len(data['dt'])):
    v.append(vprev)
    vprev += float(data['x_kalman'][i]) * data['dt'][i] * 10**-3 # Convert to float to ensure scalar addition

s = []
sprev = 0
for i in range(len(data['dt'])):
    s.append(sprev)
    sprev += v[i] * data['dt'][i] *10**-3  # 'v[i]' is already a scalar here
    
print(t[0:10])
print(v[0:10])
print(s[0:10])

# plt.figure(figsize=(10, 6))

plt.subplot(1, 3, 1)
plt.xlabel("t[s]")
plt.ylabel("Aks[m/s^2]")
plt.title("t/aks")
plt.plot(t,data['x_kalman'])
# plt.show()

plt.subplot(1, 3, 2)
plt.xlabel("t[s]")
plt.title("t/fart")
plt.ylabel("Fart[m/s]")
plt.plot(t,v)
# plt.show()

plt.subplot(1, 3, 3)
plt.xlabel("t[s]")
plt.ylabel("Pos[m]")
plt.title("t/pos")
plt.plot(t,s)
plt.tight_layout()
plt.show()