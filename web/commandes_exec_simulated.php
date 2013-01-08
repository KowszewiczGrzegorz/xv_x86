
<?php

sleep(2);

if (isset($_POST['cmd'])) {

    $response = "Component,Major,Minor,Build <br/>\n
ModelID,-1,XV25,  <br/>\n
ConfigID,2,,  <br/>\n
Serial Number,WTD32512DD,0034202,P  <br/>\n
Software,3,0,17235  <br/>\n
BatteryType,1,NIMH_12CELL,  <br/>\n
BlowerType,1,BLOWER_ORIG,  <br/>\n
BrushSpeed,1200,,  <br/>\n
BrushMotorType,1,BRUSH_MOTOR_ORIG,  <br/>\n
SideBrushType,1,SIDE_BRUSH_NONE,  <br/>\n
WheelPodType,1,WHEEL_POD_ORIG,  <br/>\n
DropSensorType,1,DROP_SENSOR_ORIG, <br/>\n 
MagSensorType,1,MAG_SENSOR_ORIG,  <br/>\n
WallSensorType,1,WALL_SENSOR_ORIG,  <br/>\n
Locale,1,LOCALE_USA,  <br/>\n
LDS Software,V2.6.15295,0000000000,  <br/>\n
LDS Serial,WTD32212AA-0142147,,  <br/>\n
LDS CPU,F2802x/c001,,  <br/>\n
BootLoader Software,17225,P,p  <br/>\n
MainBoard Vendor ID,543,,  <br/>\n
MainBoard Serial Number,000000000000000000000000,,  <br/>\n
MainBoard Software,17242,1,  <br/>\n
MainBoard Boot,16219,  <br/>\n
MainBoard Version,4,0,  <br/>\n
ChassisRev,2,,  <br/>\n
UIPanelRev,1,, <br/>\n";

    echo "<div class=\"form\" id=\"connectionLog\" style=\"cursor: pointer;\" onClick=\"hideConnectionLog();\">\n";
    echo "    <h2>Connection Log</h2>\n";
    echo "    <div class=\"square\">\n";

    echo "        <p class=\"ok\" \">\n";
    echo "        <b>Creation du socket</b><br/>\n";
    echo "            --> OK.\n";
    echo "        </p>\n";

    echo "        <p class=\"ok\">\n";
    echo "        <b>Essai de connexion</b><br/>\n";
    echo "            --> OK.\n";
    echo "        </p>\n";
    
    echo "        <p class=\"ok\">\n";
    echo "        <b>Envoi de la requete </b><br/>\n";
    echo "            --> OK.\n";
    echo "        </p>\n";
    
    echo "        <p class=\"ok\">\n";
    echo "        <b>Lecture de la reponse</b><br/>\n";
    echo "--> " . $response;
    echo "        </p>\n";
    
    echo "        <p class=\"ok\">\n";
    echo "        <b>Fermeture du socket</b>\n";
    echo "        </p>\n";
    echo "    </div>\n";
    echo "</div>\n\n";

    $r =  rand (0, 100);

    echo "<div class=\"form\">\n";
    echo "    <h2>GetVersion " . $r . "</h2>  \n";
    echo "    <div class=\"square\">\n";

    if ($r % 2 == 0)
        echo $response;
    else
        echo "42\n";
    echo "    </div>\n";
    
    echo "        <div class=\"moreButton historyButtons\" id=\"connectionLogButton\" onClick=\"showConnectionLog();\"><< connection log >></div>\n";

    echo "</div>\n";
}
?>
