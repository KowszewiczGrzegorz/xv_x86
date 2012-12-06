<html>
<head>
<title>Web interface for Neato XV-25 - Configuration</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <script language="javascript" type="text/javascript" src="js/cookies.js"></script>
    <script language="javascript" type="text/javascript">
     function setCursor() {
         var cmd_input = document.getElementById('ip');
         cmd_input.focus();
         cmd_input.select();
     }
     
     window.onload = function() {
         setCursor();
<?php
    $ip = "127.0.0.1";
    $port = 8112;

    if (isset($_POST['ip']) && isset($_POST['port'])) {
        if (strcmp($_POST['ip'], "") != 0)
            $ip = $_POST['ip'];
        if (strcmp($_POST['port'], "") != 0)
            $port = $_POST['port'];

        echo "         setCookie('" . $ip . "', '" . $port . "');\n";
    } else {
        echo "         getCookie();\n";
    }
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


<div class="form">
    <h2>Configuration de la connection</h2>
    <form id="form_id" action="configuration.php" method="post">
<?php
    echo "        IP <input type=\"text\" id=\"ip\" name=\"ip\" maxlength=\"10\" size=\"10\" value=\"" . $ip . "\" /><br/>\n";
    echo "        port <input type=\"text\" id=\"port\" name=\"port\" maxlength=\"4\" size=\"4\" value=\"" . $port . "\" /><br/>\n";
?>
        <input type="submit" class="moreButton historyButtons" id="connectionLogButton" value="OK">
    </form>
</div>
</body>
</html>