#ifndef POWER_CONTROLLER_EVERY_PAGE_H
#define POWER_CONTROLLER_EVERY_PAGE_H

const char line_0[] PROGMEM = (R"===(<!DOCTYPE html>)===");
const char line_1[] PROGMEM = (R"===(<html lang="en">)===");
const char line_2[] PROGMEM = (R"===(<head>)===");
const char line_3[] PROGMEM = (R"===(<meta charset="UTF-8">)===");
const char line_4[] PROGMEM = (R"===(<meta name="viewport" content="width=device-width, initial-scale=1">)===");
const char line_5[] PROGMEM = (R"===(<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" integrity="sha384-JcKb8q3iqJ61gNV9KGb8thSsNjpSL0n8PARn9HuZOnIxN0hoP+VmmDGMN5t9UJ0Z" crossorigin="anonymous">)===");
const char line_6[] PROGMEM = (R"===(<script   src="https://code.jquery.com/jquery-3.5.1.min.js"   integrity="sha256-9/aliU8dGd2tb6OSsuzixeV4y/faTqgFtohetphbbj0="   crossorigin="anonymous"></script>)===");
const char line_7[] PROGMEM = (R"===(<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js" integrity="sha384-B4gt1jrGC7Jh4AgTPSdUtOBvfO8shuf57BaghqFfPlYxofvL8/KUEfYiJOMMV+rV" crossorigin="anonymous"></script>)===");
const char line_8[] PROGMEM = (R"===(<script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.3/Chart.bundle.min.js" integrity="sha512-vBmx0N/uQOXznm/Nbkp7h0P1RfLSj0HQrFSzV8m7rOGyj30fYAOKHYvCNez+yM8IrfnW0TCodDEjRqf6fodf/Q==" crossorigin="anonymous"></script>)===");
const char line_9[] PROGMEM = (R"===(<title>Power Controller</title>)===");
const char line_10[] PROGMEM = (R"===(</head>)===");
const char line_11[] PROGMEM = (R"===(<body>)===");
const char line_12[] PROGMEM = (R"===(<div class="container">)===");
const char line_13[] PROGMEM = (R"===(<div class="row">)===");
const char line_14[] PROGMEM = (R"===(<div class="col-6">)===");
const char line_15[] PROGMEM = (R"===(<h4 class="text-center">Power</h4>)===");
const char line_16[] PROGMEM = (R"===(<div class="col-11">)===");
const char line_17[] PROGMEM = (R"===(<div class="row">)===");
const char line_18[] PROGMEM = (R"===(<table class="table table-sm table-borderless">)===");
const char line_19[] PROGMEM = (R"===(<thead>)===");
const char line_20[] PROGMEM = (R"===(<tr>)===");
const char line_21[] PROGMEM = (R"===(<th scope="col"></th>)===");
const char line_22[] PROGMEM = (R"===(<th scope="col">Status</th>)===");
const char line_23[] PROGMEM = (R"===(<th colspan="2" scope="col">Actions</th>)===");
const char line_24[] PROGMEM = (R"===(</tr>)===");
const char line_25[] PROGMEM = (R"===(</thead>)===");
const char line_26[] PROGMEM = (R"===(<tbody>)===");


const char* pageTop[] = {
        line_0, line_1, line_2, line_3, line_4, line_5, line_6, line_7, line_8, line_9, line_10, line_11, line_12, line_13,
        line_14, line_15, line_16, line_17, line_18, line_19, line_20, line_21, line_22, line_23, line_24, line_25, line_26
};


const char line_27[] PROGMEM = (R"===(</tbody>)===");
const char line_28[] PROGMEM = (R"===(</table>)===");
const char line_29[] PROGMEM = (R"===(</div>)===");
const char line_30[] PROGMEM = (R"===(<div class="row">)===");
const char line_31[] PROGMEM = (R"===(<canvas id="flts"></canvas>)===");
const char line_32[] PROGMEM = (R"===(</div>)===");
const char line_33[] PROGMEM = (R"===(</div>)===");
const char line_34[] PROGMEM = (R"===(</div>)===");
const char line_35[] PROGMEM = (R"===(<div class="col-6">)===");
const char line_36[] PROGMEM = (R"===(<h4 class="text-center">Battery Info</h4>)===");
const char line_37[] PROGMEM = (R"===(<div class="col-11">)===");
const char line_38[] PROGMEM = (R"===(<div class="row">)===");
const char line_39[] PROGMEM = (R"===(<table class="table table-sm table-borderless">)===");
const char line_40[] PROGMEM = (R"===(<thead>)===");
const char line_41[] PROGMEM = (R"===(<tr>)===");
const char line_42[] PROGMEM = (R"===(<th scope="col">Min Voltage</th>)===");
const char line_43[] PROGMEM = (R"===(<th scope="col">Max Voltage</th>)===");
const char line_44[] PROGMEM = (R"===(<th scope="col">Max Charge</th>)===");
const char line_45[] PROGMEM = (R"===(<th scope="col">Max Discharge</th>)===");
const char line_46[] PROGMEM = (R"===(<th scope="col">Max Capacity</th>)===");
const char line_47[] PROGMEM = (R"===(<th scope="col" colspan="2">Current Temperature</th>)===");
const char line_48[] PROGMEM = (R"===(</tr>)===");
const char line_49[] PROGMEM = (R"===(</thead>)===");
const char line_50[] PROGMEM = (R"===(<tbody>)===");
const char line_51[] PROGMEM = (R"===(<tr>)===");
const char line_52[] PROGMEM = (R"===(<td id="mv"></td>)===");
const char line_53[] PROGMEM = (R"===(<td id="xv"></td>)===");
const char line_54[] PROGMEM = (R"===(<td id="xc"></td>)===");
const char line_55[] PROGMEM = (R"===(<td id="xd"></td>)===");
const char line_56[] PROGMEM = (R"===(<td id="xp"></td>)===");
const char line_57[] PROGMEM = (R"===(<td id="t1"></td>)===");
const char line_58[] PROGMEM = (R"===(<td id="t2"></td>)===");
const char line_59[] PROGMEM = (R"===(</tr>)===");
const char line_60[] PROGMEM = (R"===(</tbody>)===");
const char line_61[] PROGMEM = (R"===(</table>)===");
const char line_62[] PROGMEM = (R"===(</div>)===");
const char line_63[] PROGMEM = (R"===(<div class="row">)===");
const char line_64[] PROGMEM = (R"===(<canvas id="tBat"></canvas>)===");
const char line_65[] PROGMEM = (R"===(</div>)===");
const char line_66[] PROGMEM = (R"===(<div class="row">)===");
const char line_67[] PROGMEM = (R"===(<canvas id="celV"></canvas>)===");
const char line_68[] PROGMEM = (R"===(</div>)===");
const char line_69[] PROGMEM = (R"===(</div>)===");
const char line_70[] PROGMEM = (R"===(</div>)===");
const char line_71[] PROGMEM = (R"===(</div>)===");
const char line_72[] PROGMEM = (R"===(<div class="row">)===");
const char line_73[] PROGMEM = (R"===(<canvas id="temp"></canvas>)===");
const char line_74[] PROGMEM = (R"===(</div>)===");
const char line_75[] PROGMEM = (R"===(<script>)===");
const char line_76[] PROGMEM = (R"===(let cT = document.getElementById('temp').getContext('2d');)===");
const char line_77[] PROGMEM = (R"===(let tLs = [];)===");
const char line_78[] PROGMEM = (R"===(let pD = [];)===");
const char line_79[] PROGMEM = (R"===(let tD = [];)===");
const char line_80[] PROGMEM = (R"===(let hD = [];)===");
const char line_81[] PROGMEM = (R"===(let tvl = [];)===");
const char line_82[] PROGMEM = (R"===(let rSocL = [];)===");
const char line_83[] PROGMEM = (R"===(let rSoc = [];)===");
const char line_84[] PROGMEM = (R"===(let cB = document.getElementById('tBat').getContext('2d');)===");
const char line_85[] PROGMEM = (R"===(let cVL = [];)===");
const char line_86[] PROGMEM = (R"===(let cVs = [];)===");
const char line_87[] PROGMEM = (R"===(let cV = document.getElementById('celV').getContext('2d');)===");
const char line_88[] PROGMEM = (R"===(let fL = [];)===");
const char line_89[] PROGMEM = (R"===(let fCs = [];)===");
const char line_90[] PROGMEM = (R"===(let cF = document.getElementById('flts').getContext('2d');)===");
const char line_91[] PROGMEM = (R"===(window.fetch('switches.json'))===");
const char line_92[] PROGMEM = (R"===(.then(response => response.json()))===");
const char line_93[] PROGMEM = (R"===(.then(data => {data["switches"].forEach((i, index) => {)===");
const char line_94[] PROGMEM = (R"===(document.getElementById('n'.concat(index)).innerText = i["name"];)===");
const char line_95[] PROGMEM = (R"===(document.getElementById('s'.concat(index)).className = i["state"] ? "alert-sm alert-success text-center" : "alert-sm alert-danger text-center";)===");
const char line_96[] PROGMEM = (R"===(document.getElementById('s'.concat(index)).innerText = i["state"] ? "On" : "Off";)===");
const char line_97[] PROGMEM = (R"===(document.getElementById('i'.concat(index)).value = i["state"] ? "0" : "1";)===");
const char line_98[] PROGMEM = (R"===(document.getElementById('b'.concat(index)).className = i["state"] ? "btn btn-block btn-danger" : "btn btn-block btn-success";)===");
const char line_99[] PROGMEM = (R"===(document.getElementById('b'.concat(index)).innerText = i["state"] ? "Off" : "On";})}))===");
const char line_100[] PROGMEM = (R"===(.then(() => {return fetch('battery.json')}))===");
const char line_101[] PROGMEM = (R"===(.then(response => response.json()))===");
const char line_102[] PROGMEM = (R"===(.then(data => {data["cellVoltages"].forEach((i) => {)===");
const char line_103[] PROGMEM = (R"===(cVL.push(i["cell"].concat(i["balancing"] ? ' - bal' : ''));)===");
const char line_104[] PROGMEM = (R"===(cVs.push(i["cellVoltage"]);)===");
const char line_105[] PROGMEM = (R"===(});)===");
const char line_106[] PROGMEM = (R"===(data["faults"].forEach((i) => {)===");
const char line_107[] PROGMEM = (R"===(fL.push(i["fault"]);)===");
const char line_108[] PROGMEM = (R"===(fCs.push(i["count"]);)===");
const char line_109[] PROGMEM = (R"===(});)===");
const char line_110[] PROGMEM = (R"===(tvl.push(data["totalVoltage"]);)===");
const char line_111[] PROGMEM = (R"===(rSocL.push('Charging: '.concat(data["charge"]).concat(' - Discharging: ').concat(data["discharge"]));)===");
const char line_112[] PROGMEM = (R"===(rSoc.push(data["remainingSOC"]);)===");
const char line_113[] PROGMEM = (R"===(document.getElementById("mv").innerText = data["minVoltage"];)===");
const char line_114[] PROGMEM = (R"===(document.getElementById("xv").innerText = data["maxVoltage"];)===");
const char line_115[] PROGMEM = (R"===(document.getElementById("xc").innerText = data["maxCharge"];)===");
const char line_116[] PROGMEM = (R"===(document.getElementById("xd").innerText = data["maxDischarge"];)===");
const char line_117[] PROGMEM = (R"===(document.getElementById("xp").innerText = data["maxPower"];)===");
const char line_118[] PROGMEM = (R"===(document.getElementById("t1").innerText = data["temp1"];)===");
const char line_119[] PROGMEM = (R"===(document.getElementById("t2").innerText = data["temp2"];)===");
const char line_120[] PROGMEM = (R"===(document.getElementById('tBat').height = 81;}))===");
const char line_121[] PROGMEM = (R"===(.then(() => {new Chart(cB, {)===");
const char line_122[] PROGMEM = (R"===(type: 'horizontalBar',)===");
const char line_123[] PROGMEM = (R"===(data: {)===");
const char line_124[] PROGMEM = (R"===(labels: tvl,)===");
const char line_125[] PROGMEM = (R"===(datasets: [{label: rSocL, backgroundColor: 'rgba(0, 0, 255, 0.5)', borderColor: 'rgba(0, 0, 255, 1)', data: rSoc}]},)===");
const char line_126[] PROGMEM = (R"===(options: {scales: {xAxes: [{ticks: {beginAtZero: true, suggestedMax: 100}}]}}});}))===");
const char line_127[] PROGMEM = (R"===(.then(() => {new Chart(cV, {)===");
const char line_128[] PROGMEM = (R"===(type: 'bar',)===");
const char line_129[] PROGMEM = (R"===(data: {)===");
const char line_130[] PROGMEM = (R"===(labels: cVL,)===");
const char line_131[] PROGMEM = (R"===(datasets: [{label: 'Cell Voltage', backgroundColor: 'rgba(0, 255, 0, 0.5)', borderColor: 'rgba(0, 255, 0, 1)', data: cVs}]},)===");
const char line_132[] PROGMEM = (R"===(options: {scales: {yAxes: [{scaleLabel: {display:true, labelString:'Volts'}, ticks: {beginAtZero: true}}]}}});}))===");
const char line_133[] PROGMEM = (R"===(.then()===");
const char line_134[] PROGMEM = (R"===(() => {new Chart(cF, {)===");
const char line_135[] PROGMEM = (R"===(type: 'horizontalBar',)===");
const char line_136[] PROGMEM = (R"===(data: {)===");
const char line_137[] PROGMEM = (R"===(labels: fL,)===");
const char line_138[] PROGMEM = (R"===(datasets: [{label: 'Faults', backgroundColor: 'rgba(255, 0, 0, 0.5)', borderColor: 'rgba(255, 0, 0, 1)', data: fCs}]},)===");
const char line_139[] PROGMEM = (R"===(});}))===");
const char line_140[] PROGMEM = (R"===(.then(() => { return fetch('sensors.json')}))===");
const char line_141[] PROGMEM = (R"===(.then(response => response.json()))===");
const char line_142[] PROGMEM = (R"===(.then(data => data["values"].forEach((i) => {)===");
const char line_143[] PROGMEM = (R"===(tLs.push(i["time"]);)===");
const char line_144[] PROGMEM = (R"===(pD.push(i["pressure"]);)===");
const char line_145[] PROGMEM = (R"===(tD.push(i["temp"]);)===");
const char line_146[] PROGMEM = (R"===(hD.push(i["humidity"]);)===");
const char line_147[] PROGMEM = (R"===(})))===");
const char line_148[] PROGMEM = (R"===(.then(() => {new Chart(cT, {)===");
const char line_149[] PROGMEM = (R"===(type: 'line',)===");
const char line_150[] PROGMEM = (R"===(data: {)===");
const char line_151[] PROGMEM = (R"===(labels: tLs,)===");
const char line_152[] PROGMEM = (R"===(datasets: [{label: 'Temperature', fill: false, backgroundColor: 'rgba(255, 0, 0, 0.5)', borderColor: 'rgba(255, 0, 0, 0.5)', yAxisID: '1', data: tD},)===");
const char line_153[] PROGMEM = (R"===({label: 'Humidity', fill: false, backgroundColor: 'rgba(0, 0, 255, 0.5)', borderColor: 'rgba(0, 0, 255, 0.5)', yAxisID: '1', data: hD},)===");
const char line_154[] PROGMEM = (R"===({label: 'Barometric Pressure', fill: false, backgroundColor: 'rgba(0, 255, 0, 0.5)', borderColor: 'rgba(0, 255, 0, 0.5)', yAxisID: '2', data: pD}]},)===");
const char line_155[] PROGMEM = (R"===(options: {)===");
const char line_156[] PROGMEM = (R"===(scales: {)===");
const char line_157[] PROGMEM = (R"===(yAxes: [{type: 'linear', position: 'left', id: '1'},)===");
const char line_158[] PROGMEM = (R"===({type: 'linear', position: 'right', id: '2'}])===");
const char line_159[] PROGMEM = (R"===(}}});}))===");
const char line_160[] PROGMEM = (R"===(.catch((error) => {)===");
const char line_161[] PROGMEM = (R"===(console.log('Error', error);)===");
const char line_162[] PROGMEM = (R"===(});)===");
const char line_163[] PROGMEM = (R"===(</script>)===");
const char line_164[] PROGMEM = (R"===(</div>)===");
const char line_165[] PROGMEM = (R"===(</body>)===");
const char line_166[] PROGMEM = (R"===(</html>)===");

const char* pageBottom[] ={
        line_27, line_28, line_29, line_30, line_31, line_32, line_33, line_34, line_35, line_36,
        line_37, line_38, line_39, line_40, line_41, line_42, line_43, line_44, line_45, line_46,
        line_47, line_48, line_49, line_50, line_51, line_52, line_53, line_54, line_55, line_56,
        line_57, line_58, line_59, line_60, line_61, line_62, line_63, line_64, line_65, line_66,
        line_67, line_68, line_69, line_70, line_71, line_72, line_73, line_74, line_75, line_76,
        line_77, line_78, line_79, line_80, line_81, line_82, line_83, line_84, line_85, line_86,
        line_87, line_88, line_89, line_90, line_91, line_92, line_93, line_94, line_95, line_96,
        line_97, line_98, line_99, line_100, line_101, line_102, line_103, line_104, line_105, line_106,
        line_107, line_108, line_109, line_110, line_111, line_112, line_113, line_114, line_115, line_116,
        line_117, line_118, line_119, line_120, line_121, line_122, line_123, line_124, line_125, line_126,
        line_127, line_128, line_129, line_130, line_131, line_132, line_133, line_134, line_135, line_136,
        line_137, line_138, line_139, line_140, line_141, line_142, line_143, line_144, line_145, line_146,
        line_147, line_148, line_149, line_150, line_151, line_152, line_153, line_154, line_155, line_156,
        line_157, line_158, line_159, line_160, line_161, line_162, line_163, line_164, line_165, line_166,
};

#endif //POWER_CONTROLLER_EVERY_PAGE_H
