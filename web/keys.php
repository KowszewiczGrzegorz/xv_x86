<?php require('login_xv.php'); ?>
<?php include 'menu.php'; ?>
<html>
<head>
    <title>Web interface for Neato XV-25 - Keys</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <link rel="icon" href="favicon.ico" type="image/x-icon" />
    <script language="javascript" type="text/javascript" src="js/keys.js"></script>
</head>
<body>
<h1>Web interface for Neato XV-25</h1>

<?php showMenu(); ?>

<div id="inMiddle">
    <div class="form">
	<h2>Keys</h2>
        <b>Movement</b>
	<div id="keyEvent"></div><br/>
	<b>Tilt</b>
	<div id="tiltEvent"</div><br/>
    </div>
</div>

</body>
</html>
