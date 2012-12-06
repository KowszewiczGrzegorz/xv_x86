<?php
header('Content-Type: text/xml'); 
echo "<?xml version=\"1.0\"?>\n";
echo "<scan>\n";

error_reporting(E_ALL);

$ip = $_COOKIE['ipCookie'];
$port = $_COOKIE['portCookie'];

$service_port = $port;
$address = gethostbyname($ip);
$error = 0;

if (!($socket = socket_create(AF_INET, SOCK_STREAM, 0)))
    $error = 1;

if (0 == $error) {
    if (!(socket_connect($socket, $address, $service_port)))
        $error = 1;
    if (1 == $error)
        }

if (0 == $error) {
    $in = "GetLDSScan\n";
    if (socket_write($socket ,$in ,strlen($in)) === false)
        $error = 1;
}

if (0 == $error) {
    $out = "";
    $response = "";
    $receivedDone = 0;
    while (0 == $receivedDone && 0 == $error) {
        if (false === socket_recv($socket, $out, 1024, MSG_WAITALL)) {
            $error = 1;
        } else {
            $response .= $out;
            if (false !== ($endOfResponse = strpos($response, ",EndOfResponse"))) {
                $receivedDone = 1;
                $response = substr($response, 0, $endOfResponse);
            }
        }
    }
}

socket_close($socket);

if (0 == $error) {
    $pos = strpos($response, "ErrorCodeHEX~") + 1;
    for ($i = 0; $i < 360; $i++) {
        $response = substr($response, $pos);
        $mesurePosDebut = strpos($response, ",") + 1;
        $mesurePosFin = strpos(substr($response, $mesurePosDebut), ",");
        $mesure[$i] = intval(substr($response, $mesurePosDebut, $mesurePosFin);
        $pos = strpos($response, "~") + 1;
    }
}

for ($i = 0; $i < 360; $i++) {
    echo "<mesure>" . $mesure[$i] . "</mesure>\n";
}
echo "<frequence>" . $frequence . "</frequence>\n";
echo "</scan>\n";
?>