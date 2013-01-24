<?php require('login_xv.php'); ?>
<?php include 'menu.php'; ?>
<html>
<head>
    <title>Web interface for Neato XV-25 - Keys</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <link rel="icon" href="favicon.ico" type="image/x-icon" />
    <script language="javascript" type="text/javascript" src="js/commandes.js"></script>
</head>
<body>
<h1>Web interface for Neato XV-25</h1>

<?php showMenu(); ?>

<div id="inMiddle">
    <div class="form">
	<h2>Keys</h2>
        <canvas id="keys" width="300" height="200">
            This text is displayed if your browser does not support HTML5 Canvas.
        </canvas>
    </div>
</div>

</body>
</html>
