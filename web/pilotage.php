<?php include 'menu.php'; ?>
<html>
<head>
    <title>Web interface for Neato XV-25 - Pilotage</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <link rel="icon" href="favicon.ico" type="image/x-icon" />
    <script language="javascript" type="text/javascript" src="js/cookies.js"></script>
    <script language="javascript" type="text/javascript" src="js/pilotage.js"></script>
    <script language="javascript" type="text/javascript">     
     window.onload = function() {
         initJoystick();
     }
    </script>
</head>
<body>
<h1>Web interface for Neato XV-25</h1>

<?php showMenu(); ?>

<?php
    if ($_COOKIE['ipCookie'] == '' || $_COOKIE['portCookie'] == '') {
        echo "<div class=\"warning\">\n";
        echo "    La configuration de la connection n'a pas été faite !<br/>\n";
        echo "    Allez sur la page <b><a href=\"configuration.php\">Configuration</a><b>\n";
        echo "</div>\n";
    }
?>

<div id="inMiddle">
    <div class="form">
        <h2>Pilotage</h2>
        <div class="centered gray">
            <canvas class="joystick" id="joystick" width="300px" height="300px">
                HTML 5 is not supported by your browser. You need it to run this page !
            </canvas> 
        </div>
    </div>
    
    <div class="form">
        <b>Position click</b>
        <div id="posClick"></div>
        <b>Commande</b>
        <div id="velocities"></div>
        <div id="realVelocities"></div>
    </div>
</div>

</body>
</html>