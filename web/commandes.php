<?php include 'menu.php'; ?>
<html>
<head>
<title>Web interface for Neato XV-25 - Commandes</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <link rel="icon" href="favicon.ico" type="image/x-icon" />
    <script language="javascript" type="text/javascript" src="js/cookies.js"></script>
    <script language="javascript" type="text/javascript" src="js/commandes.js"></script>
    <script src="http://code.jquery.com/jquery-latest.js"></script>
    <script language="javascript" type="text/javascript">
     window.onload = function() {
         setCursor();
         getCookie();
     }
    </script>
</head>
<body>
<h1>Web interface for Neato XV-25</h1>

<?php showMenu(); ?>

<img class="logo" src="img/xv-25.png" alt="neato XV-25" />

<?php
    if ($_COOKIE['ipCookie'] == '' || $_COOKIE['portCookie'] == '') {
        echo "<div class=\"warning\">\n";
        echo "    La configuration de la connection n'a pas été faite !<br/>\n";
        echo "    Allez sur la page <b><a href=\"configuration.php\">Configuration</a><b>\n";
        echo "</div>\n";
    }
?>

<div id="inMiddle">

<img src="img/loading.gif" alt="loading" id="loading_img" /><br/><br/>

<div class="form">
    <h2>Commande</h2>
    <input id="cmd" type="text" maxlength="30" size="30" onchange="sendCommand()" />
    <button class="moreButton historyButtons" id="connectionLogButton" onclick="sendCommand()">OK</button>
</div>

<div id="response">
    <div id="connectionLog"></div>
</div>

<div class="form" id="fullHistory">
    <h2>Historique</h2>
    <div id="historyList">
<!--
       <p class="square" id="historyLine">
           <a href="#" onclick="document.getElementById('cmd').value='" . $historyList[0] . "';return false;\">" . $historyList[0] . "</a>"
       </p>
-->
    </div>
    <div id="moreButtonDiv">
        <div class="moreButton historyButtons">
            <a href="javascript:showMoreHistory(2)"><< more >></a>
        </div>
        <div class="moreButton historyButtons">
            <a href="javascript:clearHistory()"/><< clear >></a>
        </div>
    </div>
</div>
</div>
</body>
</html>