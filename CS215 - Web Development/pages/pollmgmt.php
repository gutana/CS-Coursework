<?php require("../universalHeader.php"); ?>

<?php 
    if (!isset($_SESSION['uid']))
    {
        header("Location: index.php");
    }
?>

<body>
    <div class="container">
        <span class="header-left">
            <a href="index.php">
                <img src="../images/logo.png" alt="PollMaster Logo" href="index.php" />
            </a>
        </span>
        <div class="header-middle">
        <?php require("../components/toppanel.php"); ?>
        </div>
        <span class="header-right">
        <?php require("../components/headerright.php"); ?>
        </span>

        <div class="main-body-left">
            <div class="log-in-panel slide-in-right">
                <div class="button-area">
                    <a href="createpoll.php">
                        <button id="create-poll-btn">CREATE POLL</button>
                    </a>
                </div>
            </div>
        </div>
        <div class="main-body-middle">
            <div class="main-body-middle-wrapper">
                <div class="poll-management-panel">
                    <h2>Your Polls</h2>

            <?php
                Database::Init();
                $result = Database::GetPollsFromUID($_SESSION['uid']);
                Database::Close();

                for ($i = 0; $i < COUNT($result); $i++)
                {
                    $poll = new PollResult($result[$i]['poll_id'], true);
                }

            ?>

                </div>
            </div>
                
        </div>
        <div class="main-body-right">            
            <?php include("../components/recentPollsSidebar.php"); ?>
        </div>

    </div>
    <footer>
        <a href="../../index.html">
            Return to Mohammad's page
        </a>
    </footer>
</body>

</html>