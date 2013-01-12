<?php require('login_xv.php'); ?>
<?php include 'menu.php'; ?>
<?php 
    exec("sudo webcam.sh > /dev/null 2>&1 &");
?>

<html>
<head>
    <title>Web interface for Neato XV-25 - Webcam</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <link rel="icon" href="favicon.ico" type="image/x-icon" />
    <script language="javascript" type="text/javascript">
        window.onunload = function() {
            var xmlhttp;
            if (window.XMLHttpRequest) {
                xmlhttp = new XMLHttpRequest();
            } else {
                xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
            }
            xmlhttp.open("POST", "killWebcam.php", true);
            xmlhttp.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
            xmlhttp.send();
        };
    </script>
</head>
<body>
<h1>Web interface for Neato XV-25</h1>

<?php showMenu(); ?>

<div id="inMiddle">
    <div class="form" id="webcam">
	<h2>Webcam </h2>
        <!-- <img src="http://81.56.150.108:11042/webcam.mjpeg" alt="webcam"/> -->
	<embed type="application/x-vlc-plugin" name="player" autoplay="yes" loop="no" width="640" height="480" target="http://81.56.150.108:11042/webcam.mjpeg" />
    </div>
</div>

</body>
</html>
