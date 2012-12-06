<html>
<head>
    <title>Web interface for Neato XV-25 - Où suis-je ?</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <script language="javascript" type="text/javascript" src="js/cookies.js"></script>
    <script language="javascript" type="text/javascript" src="js/ou-suis-je.js"></script>
    <script language="javascript" type="text/javascript" src="js/demande-scan.js"></script>
    <script language="javascript" type="text/javascript">     
     window.onload = function() {
         xvImage = new Image();
         xvImage.src = 'img/xv25-top.png';

         refreshDrawing();
         updateScale();
         periodicFunction = setInterval(refreshDrawing, (document.getElementById("periode").value*1000));
     }
    </script>
</head>
<body>
<h1>Web interface for Neato XV-25</h1>

<div id="topnav">
<ul>
    <li><a href="commandes.php">Commandes</a></li>
    <li><a href="ou-suis-je.php">Où suis-je ?</a></li>
    <li><a href="configuration.php">Configuration</a></li>
</ul>
</div>

<?php
    if ($_COOKIE['ipCookie'] == '' || $_COOKIE['portCookie'] == '') {
        echo "<div class=\"warning\">\n";
        echo "    La configuration de la connection n'a pas été faite !<br/>\n";
        echo "    Allez sur la page <b><a href=\"configuration.php\">Configuration</a><b>\n";
        echo "</div>\n";
    }
?>

<div class="form">
    <h2>Où suis-je ?</h2>
    <div class="centered gray">

        <div class="square" id="position">
         <b>Position XV-25</b> : <div id="xPos"></div> / <div id="yPos"></div> / <div id="thetaPos"></div>
        </div>
        <canvas class="xv" id="xv" width="500" height="500">
            HTML 5 is not supported by your browser. You need it to run this page !
        </canvas> 
        <br/>
        <canvas class="xv" id="xv-scale" width="500" height="50">
        </canvas>
        <br/>
         Période <input type="text" id="periode" name="periode" maxlength="5" size="5" value="1" onchange="updatePeriod()"> (en sec)
         &nbsp;&nbsp;&nbsp;&nbsp;---&nbsp;&nbsp;&nbsp;&nbsp;
Échelle <input type="text" id="echelle" name="echelle" maxlength="5" size="5" value="50" onchange="updateScale()"> (en pixels/m)
    </div>
<?php
    $history = "";
if (isset($_POST['history'])) {
    $history = htmlspecialchars($_POST['history']);
}
if (isset($_POST['cmd']))
    $history = htmlspecialchars($_POST['cmd']) . "," . $history;
echo "        <input type=\"hidden\" id=\"history\" name=\"history\" value=\"" . $history . "\" />\n";
?>
    </form>
</div>

</body>
</html>