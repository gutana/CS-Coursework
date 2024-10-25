class Validator {

    static regex_email = /^\w+@[a-zA-Z_]+?\.[a-zA-Z]{2,3}$/;
    static regex_uname = /^[a-zA-Z0-9_-]+$/;
    static regex_pswd  = /^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[#?!@$%^&*-]).{8,}$/;
    // Alternative password one /^(\S*)?\d+(\S*)?$/;
    static regex_wspc  = /^$|\s+/;
    static regex_noSpecialChars = /^[A-Za-z0-9 ]+$/;
    // special char regex from https://stackoverflow.com/questions/6053541/regex-every-non-alphanumeric-character-except-white-space-or-colon

    static validateEmail(email) 
    {
        if (this.regex_email.test(email)) {
            return true;
        }
        return false;
    }

    static verifyLength(str, len, options) // Expected options: "exact", "minimum", "maximum"
    {
        if (options == "exact") {
            return (str.length == len)
        } 
        else if (options == "minimum") {
            return (str.length >= len) 
        } 
        else if (options == "maximum") {
            return (str.length <= len) 
        }
    }

    static verifyNoWhitespace(str)
    {
        if (str.length == 0) 
        {
            return true; 
        }
        return !this.regex_wspc.test(str);
    }

    static verifyNoSpecialChars(str) {
        if (this.regex_noSpecialChars.test(str))
        {
            return true; 
        }
        return false;
    }

    static verifyPassword(str) {
        if (this.regex_pswd.test(str))
        {
            return true; 
        } 
        return false; 
    }

}