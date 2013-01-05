<html>
<head>
<title>Web interface for Neato XV-25 - Commandes</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <script language="javascript" type="text/javascript" src="js/cookies.js"></script>
    <script src="http://code.jquery.com/jquery-latest.js"></script>
    <script language="javascript" type="text/javascript">
         var commands = new Array();
<?php
echo "        nbCommandsInHistory = 0;\n";
$nbHistory = 0;
if (isset($_POST['cmd'])) {
    $nbHistory += 1;
    echo "        nbCommandsInHistory += 1;\n";
    echo "        commands[0] = \"" . htmlspecialchars($_POST['cmd']) . "\";\n";
}
if (isset($_POST['history'])) {
    $historyList = explode (",",  htmlspecialchars($_POST['history']));
    if (count($historyList) > 1) {
        foreach ($historyList as $historyItem) {
            if (strcmp($historyItem, "") != 0) {
                if ($nbHistory <= 1) 
                    echo "        nbCommandsInHistory += 1;\n";
                echo "        commands[" . $nbHistory . "] = \"" . $historyItem . "\";\n";
                $nbHistory++;
            }
        }
    }
}
?>
        function showMoreHistory(nb) {
            i = 0;
            while (i < nb && nbCommandsInHistory < commands.length) {
                document.getElementById('historyList').innerHTML += "    <p class=\"square\" id=\"historyLine\">"
                    + "<a href=\"#\" onclick=\"document.getElementById('cmd').value='"
                    + commands[nbCommandsInHistory] + "';return false;\">"
                    + commands[nbCommandsInHistory] + "</a></p>\n";
                nbCommandsInHistory++;
                i++;
            }
            if (nbCommandsInHistory >= commands.length) {
                document.getElementById('moreButtonDiv').innerHTML = "";
            } else {
                document.getElementById('moreButtonDiv').innerHTML = ""
                    + "<div class=\"moreButton historyButtons\">\n"
                    + "<a href=\"javascript:showMoreHistory(" + (nb+2) + ")\"/>"
                    + "<< (" + (commands.length-nbCommandsInHistory) + ") more >></a>"
                    + "</div>\n";
            }
       };

       function clearHistory() {
           document.getElementById('history').value = "";
           document.getElementById('moreButtonDiv').innerHTML = "";
           document.getElementById('historyList').innerHTML = "";
           document.getElementById('fullHistory').style.display = 'none';
       };

       function setCursor () {
           var cmd_input = document.getElementById ('cmd');
           cmd_input.focus();
           cmd_input.select();
       }

       function sendCommand() {
           var cmd = document.getElementById('cmd').value;
           var xmlhttp;
           if (window.XMLHttpRequest) {
               xmlhttp = new XMLHttpRequest();
           } else {
               xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
           }
           xmlhttp.onreadystatechange = function() {
               if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                   document.getElementById("response").innerHTML = xmlhttp.responseText;
                   $("#response").slideToggle("slow");
               }
           }
           xmlhttp.open("POST", "commandes_exec.php", true);
           xmlhttp.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
           xmlhttp.send("cmd="+cmd);
       }

       window.onload = function() {
           setCursor();
           getCookie();
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
<div class="form">
    <h2>Commande</h2>
    <input id="cmd" type="text" maxlength="30" size="30" />
    <button id="connectionLogButton" value="OK" onclick="sendCommand()" />
<!--
    <button class="moreButton historyButtons" id="connectionLogButton" value="OK" onclick="sendCommand()" />
-->
</div>

<div id="response">
    <div class="form centered">Executing command ...</div>
    <div id="connectionLog"></div>
</div>

<script>
    $("#response").click(function () {
        $("#connectionLog").slideToggle("slow");
    });
</script>

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