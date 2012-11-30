<html>
<head><title>Web API Test</title></head>
<style type="text/css">
p.form {
    order:1px solid #A9A9A9; 
    padding:10px 20px 10px 20px;
    margin:10px;
    text-align:center;
    background-color:#D3D3D3;
}
</style>
<body>
<h2>Web API Test</h2>

<p class="form">
    <form action="webAPIForm.php" method="post">
       Commande : <input type="cmd" name="nom" /> <input type="submit" value="OK">
    </form>
</p>
     
<?php
error_reporting(E_ALL);

if (isset($_POST['cmd'])) {
    $service_port = 8112;
    $address = gethostbyname('127.0.0.1');

    echo "Création du socket : ";
    if (!($socket = socket_create(AF_INET, SOCK_STREAM, 0))) {
        echo "socket_create() a échoué (erreur:" . socket_strerror(socket_last_error()) . ")<br/>\n";
    } else {
        echo "OK.<br/>\n";
    }
    
    echo "Essai de connexion à '$address' sur le port '$service_port' : ";
    if (!(socket_connect($socket, $address, $service_port))) {
        echo "socket_connect() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
    } else {
        echo "OK.<br/>\n";
    }
    
    $in = htmlspecialchars($_POST['cmd']) . "\n";
    echo "Envoi de la requête '" . htmlspecialchars($_POST['cmd']) . "\\n' : ";
    if (socket_write($socket ,$in ,strlen($in)) === false) {
        echo "socket_write() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
    } else {
        echo "OK.<br/>\n";
    }
    
    echo "Lecture de la réponse : \n";
    $out = '';
    while (1) {
        if(($out = socket_read($socket, 2048)) === false) {
            echo "<br/>socket_read() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
            break;
        } else {
            echo $out;
        }
    }

    echo "Fermeture du socket...<br/>";
    socket_close($socket);
    echo "OK.<br/>\n\n";
}
?>
</body>
</html>