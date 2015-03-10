<html>
<head><title>Web API Test</title></head>
<body>
 <?php
error_reporting(E_ALL);

echo "<h2>Web API Test</h2>\n";

$service_port = 8112;
$address = gethostbyname('127.0.0.1');

echo "Cr�ation du socket : ";
if (!($socket = socket_create(AF_INET, SOCK_STREAM, 0))) {
    echo "socket_create() a �chou� (erreur:" . socket_strerror(socket_last_error()) . ")<br/>\n";
} else {
    echo "OK.<br/>\n";
}

echo "Essai de connexion � '$address' sur le port '$service_port' : ";
if (!(socket_connect($socket, $address, $service_port))) {
    echo "socket_connect() a �chou� (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
} else {
    echo "OK.<br/>\n";
}

$in = "GetVersion\n";
echo "Envoi de la requ�te 'GetVersion\\n' : ";
if (socket_write($socket ,$in ,strlen($in)) === false) {
    echo "socket_write() a �chou� (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
} else {
    echo "OK.<br/>\n";
}

echo "Lecture de la r�ponse : \n";
$out = '';
while (1) {
    if(($out = socket_read($socket, 2048)) === false) {
        echo "<br/>socket_read() a �chou� (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
        break;
    } else {
        echo $out;
    }
}

echo "Fermeture du socket...<br/>";
socket_close($socket);
echo "OK.<br/>\n\n";
?>
</body>
</html>