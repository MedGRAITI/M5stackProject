#pragma once

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Mesure CO2</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        body { font-family: Arial; text-align: center; background: #f2f2f2; }
        canvas { background: white; border: 1px solid #ccc; margin: 10px auto; }
        button { padding: 10px 20px; font-size: 18px; margin: 20px; }
    </style>
</head>
<body>
    <h1>Mesure CO2 - M5Stack</h1>
    <button onclick="startMeasurement()"> Demarrer la mesure</button>
    
    <div>
        <canvas id="scd30Chart" width="350" height="200"></canvas>
        <canvas id="scd40Chart" width="350" height="200"></canvas>
        <canvas id="sgp30Chart" width="350" height="200"></canvas>
        <canvas id="mhz16Chart" width="350" height="200"></canvas>
    </div>

<script>
let labels =[];
let scd30Data = { co2: [] };
let scd40Data = { co2: [] };
let sgp30Data = { eco2: [] };
let mhz16Data = [];

function startMeasurement() {
    fetch('/start').then(() => {
        alter("Mesure démarrée !");
    });
}

function createChart(ctx, label) {
  return new Chart(ctx, {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: label,
        borderColor: 'blue',
        fill: false,
        data: []
      }]
    },
    options: {
      responsive: false,
      animation: false,
      scales: {
        y: { beginAtZero: true },
        x: { title: { display: true, text: "Temps (s)" } }
      },
      plugins: {
        legend: { display: true }
      }
    }
  });
}

let scd30Chart = createChart(document.getElementById('scd30Chart').getContext('2d'), "SCD30 CO₂ (ppm)");
let scd40Chart = createChart(document.getElementById('scd40Chart').getContext('2d'), "SCD40 CO₂ (ppm)");
let sgp30Chart = createChart(document.getElementById('sgp30Chart').getContext('2d'), "SGP30 eCO₂ (ppm)");
let mhz16Chart = createChart(document.getElementById('mhz16Chart').getContext('2d'), "MH-Z16 CO₂ (ppm)");

setInterval(() => {
  fetch('/data')
    .then(res => res.json())
    .then(data => {
      let t = data.time;
      labels.push(t);
      if (labels.length > 20) labels.shift();

      scd30Data.co2.push(data.scd30_co2); if (scd30Data.co2.length > 20) scd30Data.co2.shift();
      scd40Data.co2.push(data.scd40_co2); if (scd40Data.co2.length > 20) scd40Data.co2.shift();
      sgp30Data.eco2.push(data.sgp30_eco2); if (sgp30Data.eco2.length > 20) sgp30Data.eco2.shift();
      mhz16Data.push(data.mhz16_co2); if (mhz16Data.length > 20) mhz16Data.shift();

      scd30Chart.data.labels = labels;
      scd30Chart.data.datasets[0].data = scd30Data.co2;
      scd30Chart.update();

      scd40Chart.data.labels = labels;
      scd40Chart.data.datasets[0].data = scd40Data.co2;
      scd40Chart.update();

      sgp30Chart.data.labels = labels;
      sgp30Chart.data.datasets[0].data = sgp30Data.eco2;
      sgp30Chart.update();

      mhz16Chart.data.labels = labels;
      mhz16Chart.data.datasets[0].data = mhz16Data;
      mhz16Chart.update();
    });
}, 5000);
</script>

</body>
</html>

)rawliteral";