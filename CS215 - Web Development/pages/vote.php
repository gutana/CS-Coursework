<?php require("../universalHeader.php"); ?>

<?php

if (isset($_GET['pid']))
{

Database::Init();
$poll = Database::QueryPoll($_GET['pid'])[0];
Database::Close();

$question = $poll['question'];
$authorID = $poll['user_id'];

$answerArray = [];

array_push($answerArray, $poll['a1']);
array_push($answerArray, $poll['a2']);

if (isset($poll['a3']))
{
    array_push($answerArray, $poll['a3']);
}
if (isset($poll['a4']))
{
    array_push($answerArray, $poll['a4']);
}
if (isset($poll['a5']))
{
    array_push($answerArray, $poll['a5']);
}

Database::Init();
$author = Database::GetUsernameFromUID($poll['user_id']);
$img = Database::GetImageFromUID($poll['user_id']);
Database::Close();
} 

else {
    header("Location: index.php");
    exit();
}

if (isset($_POST) && isset($_POST['radio']))
{
    $choice = $_POST['radio'];
    Database::Init();
    if (PM_SessionHandler::isLoggedIn())
    {
        Database::InsertVote($poll['poll_id'], $_SESSION['uid'], $choice);
    } else {

        Database::InsertAnonVote($poll['poll_id'], $choice);
    }
    Database::Close();
    $pid = $poll['poll_id'];
    header("Location: results.php?pid={$pid}");
    exit();
}

?>


<body>
    <div class="container">
        <span class="header-left">
            <a href="index.php">
                <img src="../images/logo.png" alt="PollMaster Logo" href="index.php" />
            </a>
        </span>
        <span class="header-middle">
            <?php require("../components/toppanel.php") ?>
        </span>
        <span class="header-right">
        <?php require("../components/headerright.php"); ?>
        </span>

        <div class="main-body-left">
            <?php require("../components/leftpanel.php"); ?>
        </div>

        <div class="main-body-middle">
            <?php $poll = new PollVote($question, $answerArray, $author, $img); ?>
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