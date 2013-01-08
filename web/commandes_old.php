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

<?php
if (isset($_POST['cmd'])) {
    echo "function sendCommand() {\n";
    echo "var xmlhttp;\n";
    echo "if (window.XMLHttpRequest) {\n";
    echo "xmlhttp = new XMLHttpRequest();\n";
    echo "} else {\n";
    echo "xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");\n";
    echo "}\n";
    echo "xmlhttp.onreadystatechange = function() {\n";
    echo "  if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {\n";
    echo "    document.getElementById(\"response\").innerHTML = xmlhttp.responseText;\n";
    echo "  }\n";
    echo "}\n";
    echo "xmlhttp.open(\"POST\", \"commandes_exec.php\", true);\n";
    echo "xmlhttp.setRequestHeader('Content-Type','application/x-www-form-urlencoded');\n";
    echo "xmlhttp.send(\"cmd=" . htmlspecialchars($_POST['cmd']) . "\");\n";
    echo "}";
}
?>

       window.onload = function() {
           setCursor();
           getCookie();
           <?php
           if (isset($_POST['cmd']))
               echo "sendCommand();\n";
           ?>
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
        <form id="form_id" action="commandes.php" method="post">
            <input type="hidden" id="ip" name="ip" maxlength="10" size="10" value="" />
            <input type="hidden" id="port" name="port" maxlength="4" size="4" value="" />
            <input type="text" id="cmd" name="cmd" maxlength="30" size="30" /> 
            <input type="submit" class="moreButton historyButtons" id="connectionLogButton" value="OK">
<?php
    $history = "";
if (isset($_POST['history'])) {
    $history = htmlspecialchars($_POST['history']);
}
if (isset($_POST['cmd']))
    $history = htmlspecialchars($_POST['cmd']) . "," . $history;
echo "            <input type=\"hidden\" id=\"history\" name=\"history\" value=\"" . $history . "\" />\n";
?>
    </form>
</div>

<?php
error_reporting(E_ALL);

if (isset($_POST['cmd'])) {
    echo "    <div id=\"response\">\n";
    echo "        <div class=\"form centered\">Executing command ...</div>\n";
    echo "        <div id=\"connectionLog\"></div>\n";
    echo "    </div>\n";

    echo "    <script>\n";
    echo "       $(\"#response\").click(function () {\n";
    echo "           $(\"#connectionLog\").slideToggle(\"slow\");\n";
    echo "       });\n";
    echo "    </script>\n";
}
?>

<?php
if (isset($_POST['cmd']) || isset($_POST['history'])) {
    echo "    <div class=\"form\" id=\"fullHistory\">\n";
    echo "        <h2>Historique</h2>\n";
    echo "        <div id=\"historyList\">\n";
    echo "            <p class=\"square\" id=\"historyLine\">\n";
    echo "                <a href=\"#\" onclick=\"document.getElementById('cmd').value='" . htmlspecialchars($_POST['cmd']) . "';return false;\">" . htmlspecialchars($_POST['cmd']) . "</a>\n";
    echo "            </p>\n";
    if ($nbHistory > 1) {
        echo "            <p class=\"square\" id=\"historyLine\">\n";
        echo "                <a href=\"#\" onclick=\"document.getElementById('cmd').value='" . $historyList[0] . "';return false;\">" . $historyList[0] . "</a>\n";
        echo "            </p>\n";
    }
    echo "        </div>\n";
    echo "        <div id=\"moreButtonDiv\">\n";
    if ($nbHistory > 2) {
        echo "            <div class=\"moreButton historyButtons\">\n";
        echo "                <a href=\"javascript:showMoreHistory(2)\"/><< (". ($nbHistory-2) .") more >></a>\n";
        echo "            </div>\n";
    }
    echo "        </div>\n";
    echo "        <div class=\"moreButton historyButtons\">\n";
    echo "            <a href=\"javascript:clearHistory()\"/><< clear >></a>\n";
    echo "        </div>\n";
    echo "    </div>\n";
}
?>
</div>
</body>
</html>