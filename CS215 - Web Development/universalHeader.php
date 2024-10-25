<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="stylesheet" type="text/css" href="../styles.css">

    </link>
    <link rel="icon" type="image/x-icon" href="../images/icon.png">
    </link>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    </link>
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    </link>
    <link href="https://fonts.googleapis.com/css2?family=Roboto+Condensed&amp;display=swap" rel="stylesheet">
    </link>
    <title>PollMaster</title>
</head>

<?php

session_start();

class PM_SessionHandler
{
    public static function isLoggedIn()
    {
        if (isset($_SESSION['uid'])) {
            return true;
        } else {
            return false;
        }
    }
    public static function logUserIn($username, $uid)
    {
        $_SESSION['username'] = $username;
        $_SESSION['uid'] = $uid;
    }
    public static function handleLogin()
    {
        if (isset($_POST['email']) && isset($_POST['password'])) {
            Database::Init();
            $userRow = Database::QueryLogin($_POST['email'], $_POST['password']);
            Database::Close();
            if ($userRow) {
                $user = $userRow[0]['username'];
                $uid = $userRow[0]['user_id'];
                PM_SessionHandler::logUserIn($user, $uid);
                header("Location: pollmgmt.php");
            } else {
                return "Username and password combination do not match.";
            }
        }
    }
}

class InputHasher
{
    public static function hashPassword($pass)
    {
        return hash("sha256", $pass);
    }
    public static function hashName($name)
    {
        return hash("md5", $name);
    }
}

class InputValidator
{

    public static function validateEmail($email)
    {
        return preg_match("/^\w+@[a-zA-Z_]+?\.[a-zA-Z]{2,3}$/", $email);
    }
    public static function validateUsername($username)
    {
        return preg_match("/^[A-Za-z0-9 ]+$/", $username) && preg_match("/^[a-zA-Z0-9_-]+$/", $username);
    }
    public static function validatePassword($password)
    {
        return !preg_match("/^(.{0,7}|[^0-9]*|[^A-Z]*|[^a-z]*|[a-zA-Z0-9]*)$/", $password);
        // Alternative password one /^(\S*)?\d+(\S*)?$/;
    }
    public static function verifyNoSpecialChars($string)
    {
        return preg_match("/^[A-Za-z0-9 ]+$/", $string);
    }
    public static function validateMaxLength($string, $length)
    {
        return strlen($string) <= $length;
    }
    public static function validateMinLength($string, $length)
    {
        return strlen($string) >= $length;
    }
    public static function verifyNoWhitespace($string)
    {
        return preg_match("/^$|\s+/", $string);
    }
}

class QueryResult
{
    private $stmt;

    public function __construct($stmt)
    {
        $this->stmt = $stmt;
    }
    // return assoc array or null if no rows left
    public function getRow()
    {
        $resp = $this->stmt->fetch(PDO::FETCH_ASSOC);
        return $resp;
    }
}

class Database
{
    private static $db;

    public static function Init()
    {
        if (self::$db) {
            return;
        }

        $host = 'localhost';
        $dbname = 'SECRET';
        $user = 'SECRET';
        $password = 'SECRET';

        $dsn = 'mysql:host=' . $host . ';dbname=' . $dbname;

        try {
            self::$db = new PDO($dsn, $user, $password);
        } catch (PDOException $e) {
            die("failed to connect to database: " . $e->getMessage() . "<br />");
        }
    }

    public static function Close()
    {
        self::$db = null;
    }

    public static function Query($q)
    {
        return new QueryResult(self::$db->query($q));
    }

    public static function QueryLogin($email, $password)
    {
        $passHashed = InputHasher::hashPassword($password);
        $sql = "SELECT user_id, username FROM users 
                WHERE email = ? AND pw_hash = ?";

        $stmt = self::$db->prepare($sql);
        $stmt->execute([$email, $passHashed]);

        return $stmt->fetchAll();
    }

    public static function GetPollsFromUID($uid)
    {
        $sql = "SELECT * FROM polls 
        WHERE user_id = ?
        ORDER BY start_time DESC";
        $stmt = self::$db->prepare($sql);
        $stmt->execute([$uid]);

        return $stmt->fetchAll();
    }

    public static function QueryPoll($poll_id)
    {
        $sql = "SELECT poll_id, user_id, question, a1, a2, a3, a4, a5, start_time FROM polls 
                WHERE poll_id = ?";

        $stmt = self::$db->prepare($sql);
        $stmt->execute([$poll_id]);

        return $stmt->fetchAll();
    }

    public static function GetUsernameFromUID($uid)
    {
        $sql = "SELECT username FROM users 
                WHERE user_id = ?";

        $stmt = self::$db->prepare($sql);
        $stmt->execute([$uid]);

        return $stmt->fetchAll();
    }

    public static function GetImageFromUID($uid)
    {
        $sql = "SELECT img_path FROM users 
                WHERE user_id = ?";

        $stmt = self::$db->prepare($sql);
        $stmt->execute([$uid]);

        $result = $stmt->fetchAll();
        $img = $result[0]['img_path'];

        return "uploads/" . $img;
    }

    public static function GetPollItemResultCount($pid, $index)
    {
        $adjustedIndex = $index + 1;

        $sql = "SELECT COUNT(*) FROM votes
                WHERE poll_id = ? AND vote_choice = ?";

        $stmt = self::$db->prepare($sql);
        $stmt->execute([$pid, $adjustedIndex]);
        $result = $stmt->fetchAll()[0][0];

        return $result;
    }

    public static function GetMostRecentVote($pid)
    {
        $sql = "SELECT time FROM votes
                WHERE poll_id = ?
                ORDER BY time DESC
                LIMIT 1";

        $stmt = self::$db->prepare($sql);
        $stmt->execute([$pid]);
        $result = $stmt->fetch();
        return $result;
    }

    public static function RegisterUser($username, $email, $img, $imgExtension, $password)
    {
        $hashedPass = InputHasher::hashPassword($password);

        $hashedImg = InputHasher::hashName($email);

        $newImgName = $hashedImg . "." . $imgExtension;

        $newImgPath = "uploads/" . $newImgName;

        try {
            if (move_uploaded_file($img, $newImgPath)) {
                $sql = "INSERT INTO users 
                        (username, img_path, email, pw_hash)
                        VALUES (?, ?, ?, ?);";

                $stmt = self::$db->prepare($sql);
                $stmt->execute([$username, $newImgName, $email, $hashedPass]);
            } else {
                throw new Exception("Unable to upload image.");
            }
        } catch (Exception $e) {
            print_r($e);
        }
    }

    public static function InsertPoll($question, $answerArray, $startTime, $endTime)
    {
        $quesLenOK = InputValidator::validateMaxLength($question, 100);
        $ansLenOK = true;

        $success = false;

        foreach ($answerArray as $ans) {
            $ansLenOK = $ansLenOK && InputValidator::validateMaxLength($ans, 50);
        }

        if ($quesLenOK && $ansLenOK) {
            switch (count($answerArray)) {
                case 2:
                    $sql = "INSERT INTO polls (user_id, question, a1, a2, start_time, end_time)
                    VALUES (?, ?, ?, ?, ?, ?);";
                    $stmt = self::$db->prepare($sql);
                    $success = $stmt->execute([$_SESSION['uid'], $question, $answerArray[0], $answerArray[1], $startTime, $endTime]);
                    break;
                case 3:
                    $sql = "INSERT INTO polls (user_id, question, a1, a2, a3, start_time, end_time)
                    VALUES (?, ?, ?, ?, ?, ?, ?);";
                    $stmt = self::$db->prepare($sql);
                    $success = $stmt->execute([$_SESSION['uid'], $question, $answerArray[0], $answerArray[1], $answerArray[2], $startTime, $endTime]);
                    break;
                case 4:
                    $sql = "INSERT INTO polls (user_id, question, a1, a2, a3, a4, start_time, end_time)
                    VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
                    $stmt = self::$db->prepare($sql);
                    $success = $stmt->execute([$_SESSION['uid'], $question, $answerArray[0], $answerArray[1], $answerArray[2], $answerArray[3], $startTime, $endTime]);
                    break;
                case 5:
                    $sql = "INSERT INTO polls (user_id, question, a1, a2, a3, a4, a5, start_time, end_time)
                    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
                    $stmt = self::$db->prepare($sql);
                    $success = $stmt->execute([$_SESSION['uid'], $question, $answerArray[0], $answerArray[1], $answerArray[2], $answerArray[3], $answerArray[4], $startTime, $endTime]);
                    break;
            }
        }
        return $success;
    }

    public static function InsertAnonVote($pollID, $choice)
    {
        $time = date("Y-m-d H:i:s");
        $sql = "INSERT INTO votes (poll_id, vote_choice, time)
                VALUES (?, ?, ?);";
        $stmt = self::$db->prepare($sql);
        $stmt->execute([$pollID, $choice, $time]);
    }

    public static function InsertVote($pollID, $userID, $choice)
    {
        $time = date("Y-m-d H:i:s");
        $sql = "INSERT INTO votes (poll_id, vote_choice, time, user_id)
                VALUES (?, ?, ?, ?);";
        $stmt = self::$db->prepare($sql);
        $stmt->execute([$pollID, $choice, $time, $_SESSION['uid']]);
    }
}

class PollItem
{
    public $name;
    public $numVotes;

    public function __construct($name, $votes)
    {
        $this->name = $name;
        $this->$numVotes = $votes;
    }
}

class MinimalPoll
{
    //public $pollID; 
    //public $question; 

    public function __construct($pid, $q)
    {
        //$this->pollID = $pid;
        //$this->question = $q;

        echo
        "<div class=\"recent-poll\">
        <p>{$q}</p>
        <span>
            <a href=\"vote.php?pid={$pid}\">
                <button class=\"recent-poll-button recent-poll-vote-button\">Vote</button>
            </a>
            <a href=\"results.php?pid={$pid}\">
                <button class=\"recent-poll-button recent-poll-results-button\">Results</button>
            </a>
        </span>
      </div>";
    }
}

class PollVote
{
    public function __construct($question, $answerArr, $author, $imgUrl)
    {
        $author = $author[0]['username'];

        echo "
        <div class=\"poll-results\">
        <h3>{$question}</h3>
        <form method=\"POST\" id=\"vote\">";

        for ($i = 0; $i < count($answerArr); $i++) {
            $adjustedNum = $i + 1;
            $text = $answerArr[$i];
            echo "
            <div class=\"vote-row\">
                <input value=\"{$adjustedNum}\" type=\"radio\" id=\"radio{$adjustedNum}\" name=\"radio\">
                <label for=\"radio{$adjustedNum}\">{$text}</label>
            </div>";
        }

        echo "
            <button id=\"vote-btn\">Vote</button>
            </form>
            <div class=\"align-left vote-author-text\">
                <img id=\"vote-author-image\" class=\"author-img\" src=\"../{$imgUrl}\">
                &emsp; Poll by {$author}
            </div>
            </div>";
    }
}

class PollResultItem
{
    public function __construct($name, $pid, $count, $percent)
    {
        echo "
        <div class=\"result-row\">
            <div class=\"result-text\">
                <div class=\"result-one-text\">{$name}</div>
            </div>
            <div style=\"--width: {$percent}%\" id=\"result-bar-one\" class=\"result-bar\"></div>
            <div class=\"result-count result-one-count\">{$count}</div>
        </div>
        ";
    }
}

class PollResult
{
    public function __construct($pid, $pollMgmtPage = false)
    {
        $this->pid = $pid;

        Database::Init();
        $result = Database::QueryPoll($pid)[0];
        Database::Close();

        $question = $result[2];

        $answerArray = [];

        array_push($answerArray, $result[3]);
        array_push($answerArray, $result[4]);

        if (isset($result['a3'])) {
            array_push($answerArray, $result[5]);
        }
        if (isset($result['a4'])) {
            array_push($answerArray, $result[6]);
        }
        if (isset($result['a5'])) {
            array_push($answerArray, $result[7]);
        }

        Database::Init();
        $pollAuthor = Database::GetUsernameFromUID($result['user_id'])[0][0];
        Database::Close();

        echo "
        <div class=\"poll-results\">
        <h3>
            {$question}
        </h3>

        ";

        $answerCountArray = [];
        Database::Init();
        for ($i = 0; $i < count($answerArray); $i++) {
            $tempCount = Database::GetPollItemResultCount($pid, $i);
            array_push($answerCountArray, $tempCount);
        }
        $authorImage = Database::GetImageFromUID($result['user_id']);
        Database::Close();

        $max = max($answerCountArray);

        for ($i = 0; $i < count($answerArray); $i++) {
            if ($max == 0) {
                $max = 1;
            }
            $percent = $answerCountArray[$i] / $max * 100;
            $resultItem = new PollResultItem($answerArray[$i], $pid, $answerCountArray[$i], $percent);
        }

        if ($pollMgmtPage) {
            $startDate = new DateTime($result['start_time']);
            $startDate = date_format($startDate, "F j Y, g:iA");

            Database::Init();
            $recentVote = Database::GetMostRecentVote($pid);
            Database::Close();

            if (isset($recentVote['time'])) {
                $recentVote = new DateTime($recentVote['time']);
                $recentVote = date_format($recentVote, "F j Y, g:iA");
            } else {
                $recentVote = "n/a";
            }

            echo "
            <div class=\"poll-information\">
                <div>Poll uploaded: {$startDate}</div>
                <div>Most recent vote: {$recentVote}</div>
            </div>";

            echo "
            <a href=\"results.php?pid={$pid}\">
                <button class=\"go-to-poll-btn\">GO TO POLL</button>
            </a>";
        } else {
            echo
            "<img id=\"result-author-image\" src=\"../{$authorImage}\">
            <div class=\"result-author-text\">
                Poll by {$pollAuthor}
            </div>";
        }
        echo "
        <br />
        </div>
        ";
    }
}


/* 

                    <div class="recent-poll recent-poll-management">
                        <h3>
                            What did you eat today?
                        </h3>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-one-text">Pizza</div>
                            </div>
                            <div style="--width: 100%" id="result-bar-one" class="result-bar"></div>
                            <div class="result-count result-one-count">80</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-two-text">Fries</div>
                            </div>
                            <div style="--width: 75%" id="result-bar-two" class="result-bar"></div>
                            <div class="result-count result-two-count">60</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-three-text">Sushi</div>
                            </div>
                            <div style="--width: 27.5%" id="result-bar-three" class="result-bar"></div>
                            <div class="result-count result-three-count">22</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-four-text">Tacos</div>
                            </div>
                            <div style="--width: 50%" id="result-bar-four" class="result-bar"></div>
                            <div class="result-count result-four-count">40</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-five-text">Steak</div>
                            </div>
                            <div style="--width: 37.5%" id="result-bar-five" class="result-bar"></div>
                            <div class="result-count result-five-count">30</div>
                        </div>
                        <div class="poll-information">
                            <div>Poll uploaded: May 1 2022, 2:00PM</div>
                            <div>Most recent vote: July 16 2022, 3:00PM</div>
                        </div>
                        <a href="results.php">
                            <button class="go-to-poll-btn">GO TO POLL</button>
                        </a>
                    </div>
                    <div class="recent-poll recent-poll-management">
                        <h3>
                            Favourite Sport?
                        </h3>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-one-text">Hockey</div>
                            </div>
                            <div style="--width: 100%" id="result-bar-one" class="result-bar"></div>
                            <div class="result-count result-one-count">100</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-two-text">Basketball</div>
                            </div>
                            <div style="--width: 24%" id="result-bar-two" class="result-bar"></div>
                            <div class="result-count result-two-count">24</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-three-text">Football</div>
                            </div>
                            <div style="--width: 50%" id="result-bar-three" class="result-bar"></div>
                            <div class="result-count result-three-count">50</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-four-text">Soccer</div>
                            </div>
                            <div style="--width: 78%" id="result-bar-four" class="result-bar"></div>
                            <div class="result-count result-four-count">78</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-five-text">Baseball</div>
                            </div>
                            <div style="--width: 22%" id="result-bar-five" class="result-bar"></div>
                            <div class="result-count result-five-count">22</div>
                        </div>
                        <div class="poll-information">
                            <div>Poll uploaded: January 13 2022, 4:00PM</div>
                            <div>Most recent vote: July 16 2022, 5:00PM</div>
                        </div>
                        <a href="results.php">
                            <button class="go-to-poll-btn">GO TO POLL</button>
                        </a>
                    </div>
                    <div class="recent-poll recent-poll-management">
                        <h3>
                            Best wrestler of all-time?
                        </h3>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-one-text">Hulk Hogan</div>
                            </div>
                            <div style="--width: 80%" id="result-bar-one" class="result-bar"></div>
                            <div class="result-count result-one-count">40</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-two-text">Sting</div>
                            </div>
                            <div style="--width: 66%" id="result-bar-two" class="result-bar"></div>
                            <div class="result-count result-two-count">33</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-three-text">Rick Flare</div>
                            </div>
                            <div style="--width: 44%" id="result-bar-three" class="result-bar"></div>
                            <div class="result-count result-three-count">22</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-four-text">John Cena</div>
                            </div>
                            <div style="--width: 60%" id="result-bar-four" class="result-bar"></div>
                            <div class="result-count result-four-count">30</div>
                        </div>
                        <div class="result-row">
                            <div class="result-text">
                                <div class="result-five-text">The Rock</div>
                            </div>
                            <div style="--width: 100%" id="result-bar-five" class="result-bar"></div>
                            <div class="result-count result-five-count">50</div>
                        </div>
                        <div class="poll-information">
                            <div>Poll uploaded: May 1 2021, 2:00AM</div>
                            <div>Most recent vote: July 16 2022, 3:00PM</div>
                        </div>
                        <a href="results.php">
                            <button class="go-to-poll-btn">GO TO POLL</button>
                        </a>
                    </div>




*/
?>