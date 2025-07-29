#pragma once
// organigramme de classe "diagramme"
//classe mere 
//classe affichage
//classe capteur
//avec une structure
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
    .reset-btn { background-color: #ff9900; color: white; }
    .grid-container {
      display: grid;
      grid-template-columns: repeat(2, 1fr);
      gap: 25px;
      padding: 25px;
      justify-items: center;
    }
    canvas {
      height: 350px !important;
      width: 100%;
      max-width: 600px;
      background: white;
      border-radius: 10px;
      box-shadow: 0 0 15px #00000088;
    }
    p {
      font-size: 16px;
    }
  </style>
</head>
<body>
  <div class="header">
    <img src="https://enscbp.bordeaux-inp.fr/sites/default/files/upload/logo_ims.png" alt="Logo" />
    <p id="datetime">📅 --/--/---- --:--</p>
  </div>
  <h1>Dashboard CO₂ – M5Stack</h1>
  <div class="button-container">
    <button class="start-btn" onclick="startMeasurement()">▶ Démarrer la mesure</button>
    <button class="stop-btn" onclick="stopMeasurement()">⏹ Arrêter la mesure</button>
    <button class="download-btn" onclick="downloadCSV()">⬇ Télécharger CSV</button>
    <button class="reset-btn" onclick="resetCSV()">♻ Réinitialiser CSV</button>
  </div>

  <div class="grid-container">
    <!-- SCD30 -->
    <div>
      <div style="text-align:center; font-weight:bold; margin-top:5px;">
        <span style="color:#00bfff;">🟦 CO₂ (ppm)</span> |
        <span style="color:#ff9900;">🟧 Température (°C)</span> |
        <span style="color:#66ff66;">🟩 Humidité Relative (%)</span>
      </div>
    <canvas id="scd30Chart"></canvas>
    <p id="scd30Value" style="text-align:center; font-weight:bold; margin: 8px 0;">--</p>
    <div style="text-align:center;">
      <button class="download-btn" onclick="downloadChartImage('scd30Chart', 'SCD30_chart')">📷 Télécharger PNG</button>
    </div>
  </div>

    <!-- SCD40 -->
    <div>
      <div style="text-align:center; font-weight:bold; margin-top:5px;">
        <span style="color:#ffa500;">🟧 CO₂ (ppm)</span> |
        <span style="color:#ffcc00;">🟨 Température (°C)</span> |
        <span style="color:#00cc66;">🟩 Humidité Relative (%)</span>
      </div>
      <canvas id="scd40Chart"></canvas>
        <p id="scd40Value" style="text-align:center; font-weight:bold; margin: 8px 0;">--</p>
    <div style="text-align:center;">
      <button class="download-btn" onclick="downloadChartImage('scd40Chart', 'SCD40_chart')">📷 Télécharger PNG</button>
    </div>
  </div>

    <!-- SGP30 -->
    <div>
      <div style="text-align:center; font-weight:bold; margin-top:5px;">
        <span style="color:#32cd32;">🟩 eCO₂ (ppm)</span>
      </div>
    <canvas id="sgp30Chart"></canvas>
    <p id="sgp30Value" style="text-align:center; font-weight:bold; margin: 8px 0;">--</p>
    <div style="text-align:center;">
      <button class="download-btn" onclick="downloadChartImage('sgp30Chart', 'SGP30_chart')">📷 Télécharger PNG</button>
    </div>
  </div>

    <!-- MH-Z16 -->
    <div>
      <div style="text-align:center; font-weight:bold; margin-top:5px;">
        <span style="color:#ff69b4;">🟪 MH-Z16 CO₂ (ppm)</span>
      </div>
      <canvas id="mhz16Chart"></canvas>
        <p id="mhz16Value" style="text-align:center; font-weight:bold; margin: 8px 0;">--</p>
        <div style="text-align:center;">
        <button class="download-btn" onclick="downloadChartImage('mhz16Chart', 'MHZ16_chart')">📷 Télécharger PNG</button>
    </div>
  </div>

    <!-- PM Sensor -->
    <div>
      <div style="text-align:center; font-weight:bold; margin-top:5px;">
        <span style="color:#ff0000;">🟥 PM1</span> |
        <span style="color:#00ff00;">🟩 PM2.5</span> |
        <span style="color:#0000ff;">🟦 PM10</span>
      </div>
      <canvas id="pmChart"></canvas>
      <p id="pmValue" style="text-align:center; font-weight:bold; margin: 8px 0;">--</p>
       <div style="text-align:center;">
      <button class="download-btn" onclick="downloadChartImage('pmChart', 'PM_chart')">📷 Télécharger PNG</button>
    </div>
  </div>

<script>
function updateDateTime() {
  const now = new Date();
  const formatted = now.toLocaleString('fr-FR');
  document.getElementById('datetime').textContent = "📅 " + formatted;
}
setInterval(updateDateTime, 1000);
updateDateTime();

function startMeasurement() {
  fetch('/start').then(() => alert("Mesure démarrée !"));
}
function stopMeasurement() {
  fetch('/stop').then(() => alert("Mesure arrêtée !"));
}
function downloadCSV() {
  const now = new Date();
  const dateStr = now.toISOString().slice(0, 10); 
  const timeStr = now.toTimeString().slice(0,5).replace(":", "h"); 
  const filename = `Mesure_${dateStr}_${timeStr}.csv`;

  fetch('/download')
    .then(response => response.blob())
    .then(blob => {
      const link = document.createElement('a');
      link.href = URL.createObjectURL(blob);
      link.download = filename;
      document.body.appendChild(link);
      link.click();
      document.body.removeChild(link);
    })
    .catch(err => alert("Erreur de téléchargement : " + err));
}

function resetCSV() {
  if (confirm("Réinitialiser le fichier CSV ?")) {
    fetch('/reset').then(() => alert("CSV réinitialisé !"));
  }
}

function downloadChartImage(canvasId, filename) {
  const canvas = document.getElementById(canvasId);
  const link = document.createElement('a');
  link.download = filename + '.png';
  link.href = canvas.toDataURL('image/png');
  link.click();
}

function createMultiChart(ctx, labels, colors, yTitle) {
  const datasets = labels.map((label, index) => {
    let yAxisID = (label.includes("Temp") || label.includes("Hum")) ? "y2" : "y";
    return {
      label: label,
      borderColor: colors[index],
      backgroundColor: colors[index] + '33',
      fill: true,
      tension: 0.4,
      pointRadius: 3,
      data: [],
      yAxisID: yAxisID
    };
  });

  return new Chart(ctx, {
    type: 'line',
    data: { labels: [], datasets: datasets },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      animation: false,
      scales: {
        x: {
          ticks: {
            color: '#ccc',
            callback: function(value) {
              const date = new Date(value * 1000);
              return date.toLocaleTimeString('fr-FR', { hour12: false });
            }
          },
          grid: { display: false },
          title: { display: true, text: 'Temps (hh:mm:ss)', color: '#ccc', font: { weight: 'bold' } }
        },
        y: {
          type: 'linear',
          position: 'left',
          title: { display: true, text: yTitle, color: '#ccc', font: { weight: 'bold' } },
          ticks: { color: '#ccc' },
          grid: { display: false }
        },
        y2: {
          type: 'linear',
          position: 'right',
          title: { display: true, text: 'Temp (°C) / RH (%)', color: '#ccc', font: { weight: 'bold' } },
          ticks: { color: '#ccc' },
          grid: { drawOnChartArea: false }
        }
      },
      plugins: {
        legend: { display: false }
      }
    }
  });
}

let labels = [];
let scd30Data = { co2: [], temp: [], hum: [] };
let scd40Data = { co2: [], temp: [], hum: [] };
let sgp30Data = { eco2: [] };
let mhz16Data = [];
let pmData = { pm1: [], pm2_5: [], pm10: [] };

let scd30Chart = createMultiChart(document.getElementById('scd30Chart').getContext('2d'),
  ["CO₂ (ppm)", "Température (°C)", "Humidité (%)"],
  ["#00bfff", "#ff9900", "#66ff66"], "CO₂ (ppm)");

let scd40Chart = createMultiChart(document.getElementById('scd40Chart').getContext('2d'),
  ["CO₂ (ppm)", "Température (°C)", "Humidité (%)"],
  ["#ffa500", "#ffcc00", "#00cc66"], "CO₂ (ppm)");

let sgp30Chart = createMultiChart(document.getElementById('sgp30Chart').getContext('2d'),
  ["eCO₂ (ppm)"], ["#32cd32"], "eCO₂ (ppm)");

let mhz16Chart = createMultiChart(document.getElementById('mhz16Chart').getContext('2d'),
  ["MH-Z16 CO₂ (ppm)"], ["#ff69b4"], "CO₂ (ppm)");

let pmChart = createMultiChart(document.getElementById('pmChart').getContext('2d'),
  ["PM1", "PM2.5", "PM10"], ["#ff0000", "#00ff00", "#0000ff"], "µg/m³");

setInterval(() => {
  fetch('/data')
    .then(res => res.json())
    .then(data => {
      const t = data.time;
      labels.push(t);
      if (labels.length > 20) labels.shift();

      scd30Data.co2.push(data.scd30_co2 || 0);
      scd30Data.temp.push(data.scd30_temp || 0);
      scd30Data.hum.push(data.scd30_hum || 0);
      if (scd30Data.co2.length > 20) { scd30Data.co2.shift(); scd30Data.temp.shift(); scd30Data.hum.shift(); }

      scd40Data.co2.push(data.scd40_co2 || 0);
      scd40Data.temp.push(data.scd40_temp || 0);
      scd40Data.hum.push(data.scd40_hum || 0);
      if (scd40Data.co2.length > 20) { scd40Data.co2.shift(); scd40Data.temp.shift(); scd40Data.hum.shift(); }

      sgp30Data.eco2.push(data.sgp30_eco2 || 0);
      if (sgp30Data.eco2.length > 20) sgp30Data.eco2.shift();

      mhz16Data.push(data.mhz16_co2 || 0);
      if (mhz16Data.length > 20) mhz16Data.shift();

      pmData.pm1.push(data.pm1 || 0);
      pmData.pm2_5.push(data.pm2_5 || 0);
      pmData.pm10.push(data.pm10 || 0);
      if (pmData.pm1.length > 20) { pmData.pm1.shift(); pmData.pm2_5.shift(); pmData.pm10.shift(); }

      scd30Chart.data.labels = labels;
      scd30Chart.data.datasets[0].data = scd30Data.co2;
      scd30Chart.data.datasets[1].data = scd30Data.temp;
      scd30Chart.data.datasets[2].data = scd30Data.hum;
      scd30Chart.update();

      scd40Chart.data.labels = labels;
      scd40Chart.data.datasets[0].data = scd40Data.co2;
      scd40Chart.data.datasets[1].data = scd40Data.temp;
      scd40Chart.data.datasets[2].data = scd40Data.hum;
      scd40Chart.update();

      sgp30Chart.data.labels = labels;
      sgp30Chart.data.datasets[0].data = sgp30Data.eco2;
      sgp30Chart.update();

      mhz16Chart.data.labels = labels;
      mhz16Chart.data.datasets[0].data = mhz16Data;
      mhz16Chart.update();

      pmChart.data.labels = labels;
      pmChart.data.datasets[0].data = pmData.pm1;
      pmChart.data.datasets[1].data = pmData.pm2_5;
      pmChart.data.datasets[2].data = pmData.pm10;
      pmChart.update();

      document.getElementById('scd30Value').textContent = `CO₂: ${scd30Data.co2.at(-1)} ppm | Temp: ${scd30Data.temp.at(-1)} °C | Hum: ${scd30Data.hum.at(-1)} %`;
      document.getElementById('scd40Value').textContent = `CO₂: ${scd40Data.co2.at(-1)} ppm | Temp: ${scd40Data.temp.at(-1)} °C | Hum: ${scd40Data.hum.at(-1)} %`;
      document.getElementById('sgp30Value').textContent = `eCO₂: ${sgp30Data.eco2.at(-1)} ppm`;
      document.getElementById('mhz16Value').textContent = `MH-Z16 CO₂: ${mhz16Data.at(-1)} ppm`;
      document.getElementById('pmValue').textContent = `PM1: ${pmData.pm1.at(-1)} µg/m³ | PM2.5: ${pmData.pm2_5.at(-1)} µg/m³ | PM10: ${pmData.pm10.at(-1)} µg/m³`;

    })
    .catch(error => console.error("❌ Erreur fetch /data :", error));
}, 5000);
</script>
</body>
</html>
)rawliteral";
