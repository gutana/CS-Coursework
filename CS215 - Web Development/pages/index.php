<?php require("../universalHeader.php"); ?>

<?php

$loginErrorText = "";

if (isset($_POST) && !PM_SessionHandler::isLoggedIn()) {
    PM_SessionHandler::handleLogin();
}


?>

<body>
    <div class="container">
        <span class="header-left">
            <a href="index.php">
                <img src="../images/logo.png" alt="PollMaster Logo" href="index.php" />
            </a>
        </span>

        <!-- TOP PANEL GOES HERE -->
        <span class="header-middle">
            <?php require("../components/toppanel.php"); ?>
        </span>

        <span class="header-right">
            <?php require("../components/headerright.php"); ?>
        </span>


        <div class="main-body-left">
            <?php require("../components/leftpanel.php"); ?>
        </div>

        <div class="main-body-middle">
            <?php include("../components/homePageText.php"); ?>
        </div>
        <div class="main-body-right">
            <?php include("../components/recentPollsSidebar.php"); ?>
        </div>

    </div>
    <footer>
        <a href="../index.html">
            Return to Mohammad's page
        </a>
    </footer>
</body>

</html>