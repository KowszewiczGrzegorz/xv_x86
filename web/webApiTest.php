<html>
<head><title>Web API Test</title></head>
<body>
 <?php
error_reporting(E_ALL);

echo "<h2>Web API Test</h2>\n";

/* Lit le port du service WWW. */
// $service_port = getservbyname('www', 'tcp');
$service_port = 8112;

/* Lit l'adresse IP du serveur de destination */
$address = gethostbyname('127.0.0.1');

/* Cr�e un socket TCP/IP. */
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($socket === false) {
    echo "socket_create() a �chou� : raison :  " . socket_strerror(socket_last_error()) . "<br/>\n";
} else {
    echo "OK.<br/>\n";
}

echo "Essai de connexion � '$address' sur le port '$service_port'...<br/>";
$result = socket_connect($socket, $address, $service_port);
if ($socket === false) {
    echo "socket_connect() a �chou� : raison : ($result) " . socket_strerror(socket_last_error($socket)) . "<br/>\n";
} else {
    echo "OK.<br/>\n";
}

$in = "GetVersion\n";

echo "Envoi de la requ�te '$in'<br/>\n";
socket_write($socket, $in, strlen($in));
echo "OK.<br/>\n";

echo "Lire la r�ponse : <br/>\n";
$out = '';
while ($out = socket_read($socket, 2048)) {
    echo $out;
}

echo "Fermeture du socket...<br/>";
socket_close($socket);
echo "OK.<br/>\n\n";
?>
</body>
</html>