#pragma once

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Dashboard CO₂ – M5Stack</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    body {
      font-family: 'Segoe UI', sans-serif;
      margin: 0;
      padding: 0;
      background-color: #10142a;
      color: white;
    }
    .header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 10px 20px;
    }
    .header img {
      width: 120px;
    }
    .header #datetime {
      font-size: 16px;
    }
    h1 {
      text-align: center;
      font-size: 28px;
      margin-top: 10px;
    }
    .button-container {
      text-align: center;
      margin: 20px;
    }
    button {
      padding: 12px 20px;
      margin: 10px;
      font-size: 16px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      transition: 0.3s;
    }
    button:hover {
      opacity: 0.9;
    }
    .start-btn { background-color: #00bfff; color: white; }
    .stop-btn { background-color: #ff5050; color: white; }
    .download-btn { background-color: #4caf50; color: white; }
    .grid-container {
      display: grid;
      grid-template-columns: repeat(2, 1fr);
      gap: 20px;
      padding: 20px;
      justify-items: center;
    }
    canvas {
      height: 250px !important;
      width: 100%;
      max-width: 450px;
      background: white;
      border-radius: 10px;
      box-shadow: 0 0 10px #00000088;
    }
  </style>
</head>
<body>
  <div class="header">
    <img src="https://example.com/logo.png" alt="Logo" />
    <p id="datetime">📅 --/--/---- --:--</p>
  </div>
  <h1>Dashboard CO₂ – M5Stack</h1>
  <div class="button-container">
    <button class="start-btn" onclick="startMeasurement()">▶ Démarrer la mesure</button>
    <button class="stop-btn" onclick="stopMeasurement()">⏹ Arrêter la mesure</button>
    <button class="download-btn" onclick="downloadCSV()">⬇ Télécharger CSV</button>
  </div>
  <div class="grid-container">
    <canvas id="scd30Chart"></canvas>
    <canvas id="scd40Chart"></canvas>
    <canvas id="sgp30Chart"></canvas>
    <canvas id="mhz16Chart"></canvas>
  </div>
<script>
let labels = [];
let scd30Data = { co2: [] };
let scd40Data = { co2: [] };
let sgp30Data = { eco2: [] };
let mhz16Data = [];
function startMeasurement() {
  fetch('/start').then(() => alert("Mesure démarrée !"));
}
function stopMeasurement() {
  fetch('/stop').then(() => alert("Mesure arrêtée !"));
}
function downloadCSV() {
  window.open('/download', '_blank');
}
function updateDateTime() {
  const now = new Date();
  const formatted = now.toLocaleString('fr-FR');
  document.getElementById('datetime').textContent = "📅 " + formatted;
}
setInterval(updateDateTime, 1000);
updateDateTime();
function createChart(ctx, label, color) {
  return new Chart(ctx, {
    type: 'line',
    data: {
      labels: [],
      datasets: [{
        label: label,
        borderColor: color,
        backgroundColor: color + '33',
        fill: true,
        tension: 0.4,
        pointRadius: 2,
        data: []
      }]
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      animation: false,
      scales: {
        y: {
          beginAtZero: true,
          ticks: { color: '#ddd' },
          grid: { color: '#333' }
        },
        x: {
          ticks: { color: '#ddd' },
          grid: { color: '#333' }
        }
      },
      plugins: {
        legend: {
          labels: { color: color }
        }
      }
    }
  });
}
let scd30Chart = createChart(document.getElementById('scd30Chart').getContext('2d'), "SCD30 CO₂ (ppm)", "#00bfff");
let scd40Chart = createChart(document.getElementById('scd40Chart').getContext('2d'), "SCD40 CO₂ (ppm)", "#ffa500");
let sgp30Chart = createChart(document.getElementById('sgp30Chart').getContext('2d'), "SGP30 eCO₂ (ppm)", "#32cd32");
let mhz16Chart = createChart(document.getElementById('mhz16Chart').getContext('2d'), "MH-Z16 CO₂ (ppm)", "#ff69b4");
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

