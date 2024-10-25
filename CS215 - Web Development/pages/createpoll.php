<?php require("../universalHeader.php"); ?>

<?php

if (PM_SessionHandler::isLoggedIn() == false) {
    header("Location: index.php");
    exit();
}

if (isset($_POST) && isset($_POST['poll-question'])) {
    $question = $_POST['poll-question'];

    $startTime;
    $endTime;

    switch ($_POST['start-option']) {
        case "start-now":
            $startTime = date("Y-m-d H:i:s");
            break;
        case "start-custom":
            $startTime = $_POST["custom-start-date-input"];
            $startTime[10] = " ";
            $startTime = $startTime . ":00";
            break;
    }

    switch ($_POST["end-option"]) {
        case "end-one-day":
            $date = new DateTime($startTime);
            $interval = new DateInterval("P1D"); // period of 1 month
            $date = date_add($date, $interval);
            $endTime = $date->format("Y-m-d H:i:s");
            break;
        case "end-one-week":
            $date = new DateTime($startTime);
            $interval = new DateInterval("P1W"); // period of 1 week
            $date = date_add($date, $interval);
            $endTime = $date->format("Y-m-d H:i:s");
            break;
        case "end-one-month":
            $date = new DateTime($startTime);
            $interval = new DateInterval("P1M"); // period of 1 month
            $date = date_add($date, $interval);
            $endTime = $date->format("Y-m-d H:i:s");
            break;
        case "end-custom":
            $endTime = $_POST["custom-end-date-input"];
            $endTime[10] = " ";
            $endTime = $endTime . ":00";
            break;
    }

    $answerArray = [];
    $a1 = "";
    $a2 = "";
    $a3 = "";
    $a4 = "";
    $a5 = "";

    if (isset($_POST['a1']) && $_POST['a1'] != "") {
        array_push($answerArray, filter_input(INPUT_POST, 'a1', FILTER_SANITIZE_SPECIAL_CHARS));
    }
    if (isset($_POST['a2']) && $_POST['a2'] != "") {
        array_push($answerArray, filter_input(INPUT_POST, 'a2', FILTER_SANITIZE_SPECIAL_CHARS));
    }
    if (isset($_POST['a3']) && $_POST['a3'] != "") {
        array_push($answerArray, filter_input(INPUT_POST, 'a3', FILTER_SANITIZE_SPECIAL_CHARS));
    }
    if (isset($_POST['a4']) && $_POST['a4'] != "") {
        array_push($answerArray, filter_input(INPUT_POST, 'a4', FILTER_SANITIZE_SPECIAL_CHARS));
    }
    if (isset($_POST['a5']) && $_POST['a5'] != "") {
        array_push($answerArray, filter_input(INPUT_POST, 'a5', FILTER_SANITIZE_SPECIAL_CHARS));
    }

    $errorText = "";

    Database::Init();
    if (Database::InsertPoll($question, $answerArray, $startTime, $endTime)) {
        header("Location: pollmgmt.php");
    } else {
        $errorText = "Error posting poll.";
    }

    Database::Close();
}
?>


<script src="../scripts/validator.js"></script>
<script src="../scripts/createPoll.js" defer></script>
<script src="../scripts/charCount.js"></script>

<body>
    <div class="container">
        <span class="header-left">
            <a href="index.php">
                <img src="../images/logo.png" alt="PollMaster Logo" href="index.php" />
            </a>
        </span>
        <span class="header-middle">
            <?php require('../components/toppanel.php'); ?>
        </span>
        <span class="header-right">
            <?php require("../components/headerright.php"); ?>
        </span>

        <div class="main-body-left">
            <?php require("../components/leftpanel.php"); ?>
        </div>
        <div class="main-body-middle">

            <div class="recent-poll" id="create-poll-div">
                <h2>CREATE A POLL</h2>
                <form method='post' id="create-poll-form">
                    <label for="poll-question">Poll question:</label> <br />
                    <div class="centered-inputs">
                        <div>
                            <input id="poll-input-question" class="poll-input poll-question-input" name="poll-question"><span class="char-counter-text">100</span>
                            <br />
                        </div>
                    </div>
                    <label for="poll-options">Poll choices: (min 2, max 5)</label> <br />
                    <div class="centered-inputs">
                        <div>
                            <input name="a1" id="poll-choice-1" class="poll-input poll-choice-input"><span class="char-counter-text">50</span> <br />
                        </div>
                        <div>
                            <input name="a2" id="poll-choice-2" class="poll-input poll-choice-input"><span class="char-counter-text">50</span> <br />
                        </div>
                        <div>
                            <input name="a3" id="poll-choice-3" class="poll-input poll-choice-input"><span class="char-counter-text">50</span> <br />
                        </div>
                        <div>
                            <input name="a4" id="poll-choice-4" class="poll-input poll-choice-input"><span class="char-counter-text">50</span> <br />
                        </div>
                        <div>
                            <input name="a5" id="poll-choice-5" class="poll-input poll-choice-input"><span class="char-counter-text">50</span> <br />
                        </div>
                    </div>
                    <div class="date-div">

                        <label for="start-time">Start date/time:</label>
                        <select name="start-option" id="start-option">
                            <option value="start-now">Start Now</option>
                            <option value="start-custom">Custom Start Time</option>
                        </select>
                        <br />
                        <div class="centered-inputs">
                            <input name="custom-start-date-input" id="start-time" type="datetime-local"> <br />
                        </div>
                        <label for="end-time">End date/time:</label>
                        <select name="end-option" id="end-option">
                            <option value="end-one-day">One Day</option>
                            <option value="end-one-week" selected>One Week</option>
                            <option value="end-one-month">One Month</option>
                            <option value="end-custom">Custom End Time</option>
                        </select>
                        <br />
                        <div class="centered-inputs">
                            <input name="custom-end-date-input" id="end-time" type="datetime-local">
                        </div>
                    </div>
                    <div class="centered-inputs">
                        <button id="poll-submit-btn">Submit</button>
                    </div>
                    <div id="poll-error-text"><?= $errorText ?? NULL ?></div>
                </form>
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