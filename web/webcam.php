<?php require('login_xv.php'); ?>
<?php include 'menu.php'; ?>
<?php 
    exec("sudo /usr/sbin/webcam.sh > /dev/null 2>&1 &");
    sleep(1);
?>
<html>
<head>
    <title>Web interface for Neato XV-25 - Webcam</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <link rel="icon" href="favicon.ico" type="image/x-icon" />
</head>
<body>
<h1>Web interface for Neato XV-25</h1>

<?php showMenu(); ?>

<div id="inMiddle">
    <div class="form" id="webcam">
	<h2>Webcam </h2>
        <img src="http://81.56.150.108:443/?action=stream" width="640" height="480" alt="webcam"/>
        <br/><br/>
    </div>
</div>

</body>
</html>
