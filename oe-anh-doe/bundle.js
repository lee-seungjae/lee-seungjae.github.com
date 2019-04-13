/******/ (function(modules) { // webpackBootstrap
/******/ 	// The module cache
/******/ 	var installedModules = {};
/******/
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/
/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId]) {
/******/ 			return installedModules[moduleId].exports;
/******/ 		}
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			i: moduleId,
/******/ 			l: false,
/******/ 			exports: {}
/******/ 		};
/******/
/******/ 		// Execute the module function
/******/ 		modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);
/******/
/******/ 		// Flag the module as loaded
/******/ 		module.l = true;
/******/
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/
/******/
/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;
/******/
/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;
/******/
/******/ 	// define getter function for harmony exports
/******/ 	__webpack_require__.d = function(exports, name, getter) {
/******/ 		if(!__webpack_require__.o(exports, name)) {
/******/ 			Object.defineProperty(exports, name, { enumerable: true, get: getter });
/******/ 		}
/******/ 	};
/******/
/******/ 	// define __esModule on exports
/******/ 	__webpack_require__.r = function(exports) {
/******/ 		if(typeof Symbol !== 'undefined' && Symbol.toStringTag) {
/******/ 			Object.defineProperty(exports, Symbol.toStringTag, { value: 'Module' });
/******/ 		}
/******/ 		Object.defineProperty(exports, '__esModule', { value: true });
/******/ 	};
/******/
/******/ 	// create a fake namespace object
/******/ 	// mode & 1: value is a module id, require it
/******/ 	// mode & 2: merge all properties of value into the ns
/******/ 	// mode & 4: return value when already ns object
/******/ 	// mode & 8|1: behave like require
/******/ 	__webpack_require__.t = function(value, mode) {
/******/ 		if(mode & 1) value = __webpack_require__(value);
/******/ 		if(mode & 8) return value;
/******/ 		if((mode & 4) && typeof value === 'object' && value && value.__esModule) return value;
/******/ 		var ns = Object.create(null);
/******/ 		__webpack_require__.r(ns);
/******/ 		Object.defineProperty(ns, 'default', { enumerable: true, value: value });
/******/ 		if(mode & 2 && typeof value != 'string') for(var key in value) __webpack_require__.d(ns, key, function(key) { return value[key]; }.bind(null, key));
/******/ 		return ns;
/******/ 	};
/******/
/******/ 	// getDefaultExport function for compatibility with non-harmony modules
/******/ 	__webpack_require__.n = function(module) {
/******/ 		var getter = module && module.__esModule ?
/******/ 			function getDefault() { return module['default']; } :
/******/ 			function getModuleExports() { return module; };
/******/ 		__webpack_require__.d(getter, 'a', getter);
/******/ 		return getter;
/******/ 	};
/******/
/******/ 	// Object.prototype.hasOwnProperty.call
/******/ 	__webpack_require__.o = function(object, property) { return Object.prototype.hasOwnProperty.call(object, property); };
/******/
/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "";
/******/
/******/
/******/ 	// Load entry module and return exports
/******/ 	return __webpack_require__(__webpack_require__.s = "./build/oe-anh-doe/src/main.js");
/******/ })
/************************************************************************/
/******/ ({

/***/ "./build/oe-anh-doe/src/Generator.js":
/*!*******************************************!*\
  !*** ./build/oe-anh-doe/src/Generator.js ***!
  \*******************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";

Object.defineProperty(exports, "__esModule", { value: true });
function generateProblemList(rawData, count) {
    return shuffle(rawData)
        .slice(0, count)
        .map(function (v) { return createProblem(parse(v)); });
}
exports.generateProblemList = generateProblemList;
var Piece = /** @class */ (function () {
    function Piece() {
    }
    return Piece;
}());
function shuffle(a) {
    a = a.slice();
    for (var i = a.length - 1; i > 0; --i) {
        var j = randomInt(i + 1);
        var x = a[i];
        a[i] = a[j];
        a[j] = x;
    }
    return a;
}
function parse(orig) {
    var rv = [];
    for (var _i = 0, _a = orig.match(/{[^}]+}|<[^>]+>|[^{}<>]+/g); _i < _a.length; _i++) {
        var word = _a[_i];
        rv.push({
            candidates: word.match(/[^{}<>|]+/g),
            onlyFirstIsCorrect: (word.charAt(0) == '{')
        });
    }
    //console.log(orig);
    //console.log(rv);
    return rv;
}
function createProblem(pieces) {
    var q = '';
    var a = '';
    for (var _i = 0, pieces_1 = pieces; _i < pieces_1.length; _i++) {
        var piece = pieces_1[_i];
        var candidates = piece.candidates;
        var selectedPiece = candidates[randomInt(candidates.length)];
        q += selectedPiece;
        a += piece.onlyFirstIsCorrect ? candidates[0] : selectedPiece;
    }
    return {
        questionText: q,
        rightAnswer: a
    };
}
// [0, n)
function randomInt(n) {
    return Math.floor(Math.random() * n);
}
//# sourceMappingURL=Generator.js.map

/***/ }),

/***/ "./build/oe-anh-doe/src/ModalDialog.js":
/*!*********************************************!*\
  !*** ./build/oe-anh-doe/src/ModalDialog.js ***!
  \*********************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";

Object.defineProperty(exports, "__esModule", { value: true });
var ModalDialog = /** @class */ (function () {
    function ModalDialog(nodeId, animationStyle, buttonCaption) {
        var _this = this;
        this.animationStyle = animationStyle;
        this.$background = $('#modalBackground');
        var child = function (cname) { return $("#modalBackground " + cname); };
        this.$window = child('#' + nodeId);
        this.$closeButton = child('#closeButton');
        this.$closeButton.text(buttonCaption);
        this.$closeButton.click(function () { return _this.onClose(); });
        this.$closeButton.focus();
    }
    ModalDialog.prototype.onEnterKey = function () {
        this.onClose();
    };
    ModalDialog.prototype.findChild = function (selector) {
        return this.$window.find(selector);
    };
    ModalDialog.prototype.show = function (yes) {
        if (yes) {
            this.$background.show();
            this.$window.css('animation', this.animationStyle);
            this.$window.show();
        }
        else {
            this.$background.hide();
            this.$window.hide();
        }
    };
    return ModalDialog;
}());
exports.ModalDialog = ModalDialog;
//# sourceMappingURL=ModalDialog.js.map

/***/ }),

/***/ "./build/oe-anh-doe/src/ModalWindow.js":
/*!*********************************************!*\
  !*** ./build/oe-anh-doe/src/ModalWindow.js ***!
  \*********************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";

Object.defineProperty(exports, "__esModule", { value: true });
var ModalWindowStack = /** @class */ (function () {
    function ModalWindowStack() {
        var _this = this;
        this.windows = [];
        document.onkeypress = (function (event) {
            if (event.keyCode === 13) {
                event.stopPropagation();
                event.preventDefault();
                var len = _this.windows.length;
                if (len == 0) {
                    return;
                }
                // 콜백 실행 {
                // 이 안에서 this.windows가 바뀔 수 있음.
                _this.windows[len - 1].onEnterKey();
                // 콜백 실행 }
            }
        });
    }
    ModalWindowStack.prototype.showAndPush = function (wnd) {
        console.assert(this.windows.findIndex(function (v) { return v == wnd; }) < 0);
        this.windows.push(wnd);
        wnd.show(true);
    };
    ModalWindowStack.prototype.hideAndPop = function (wnd) {
        var len = this.windows.length;
        console.assert(len > 0);
        console.assert(this.windows[len - 1] == wnd);
        this.windows.pop();
        wnd.show(false);
    };
    return ModalWindowStack;
}());
exports.ModalWindowStack = ModalWindowStack;
//# sourceMappingURL=ModalWindow.js.map

/***/ }),

/***/ "./build/oe-anh-doe/src/Model.js":
/*!***************************************!*\
  !*** ./build/oe-anh-doe/src/Model.js ***!
  \***************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";

Object.defineProperty(exports, "__esModule", { value: true });
var Model = /** @class */ (function () {
    function Model(problems) {
        this.retryCounts = [];
        this.thisProblemRetryCount = 0;
        this.problems = problems;
    }
    Model.prototype.goToStart = function () {
        this.retryCounts = [];
        this.thisProblemRetryCount = 0;
    };
    Model.prototype.retry = function () {
        ++this.thisProblemRetryCount;
    };
    Model.prototype.getCurrentProblemNumber = function () {
        return this.retryCounts.length + 1;
    };
    Model.prototype.getTotalProblemCount = function () {
        return this.problems.length;
    };
    Model.prototype.next = function () {
        if (!this.isEnded()) {
            this.retryCounts.push(this.thisProblemRetryCount);
        }
        this.thisProblemRetryCount = 0;
    };
    Model.prototype.getCurrentProblem = function () {
        return this.problems[this.getCurrentProblemNumber() - 1];
    };
    Model.prototype.isEnded = function () {
        return this.retryCounts.length == this.problems.length;
    };
    Model.prototype.wasPerfect = function () {
        console.assert(this.isEnded());
        for (var _i = 0, _a = this.retryCounts; _i < _a.length; _i++) {
            var c = _a[_i];
            if (c > 0) {
                return false;
            }
        }
        return true;
    };
    return Model;
}());
exports.Model = Model;
//# sourceMappingURL=Model.js.map

/***/ }),

/***/ "./build/oe-anh-doe/src/ProblemView.js":
/*!*********************************************!*\
  !*** ./build/oe-anh-doe/src/ProblemView.js ***!
  \*********************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";

Object.defineProperty(exports, "__esModule", { value: true });
var ProblemView = /** @class */ (function () {
    function ProblemView(model) {
        var _this = this;
        this.model = model;
        this.$root = $('#problemDlg');
        this.$question = $('#problemDlg #question');
        this.$answer = $('#problemDlg #answer');
        this.$enterButton = $('#problemDlg #enterButton');
        this.$currentProblemNumber = $('#problemDlg #currentProblemNumber');
        this.$totalProblemCount = $('#problemDlg #totalProblemCount');
        this.$enterButton.click(function () { return _this.onEnterKey(); });
        this.$answer.keyup(function () { _this.updateQuestionText(); _this.updateEnterButton(); });
    }
    ProblemView.prototype.onEnterKey = function () {
        if (this.canEnter()) {
            this.onEnter();
        }
    };
    ProblemView.prototype.setUpQuestion = function () {
        this.$currentProblemNumber.text(this.model.getCurrentProblemNumber());
        this.$totalProblemCount.text(this.model.getTotalProblemCount());
        this.$answer.val('');
        this.updateQuestionText();
        this.updateEnterButton();
    };
    ProblemView.prototype.updateQuestionText = function () {
        var problem = this.model.getCurrentProblem();
        if (problem == null) {
            return;
        }
        var qtext = problem.questionText;
        var atext = this.getAnswer();
        this.$question.empty();
        for (var i = 0; i < qtext.length; ++i) {
            var qchar = qtext.charAt(i);
            var achar = atext.charAt(i);
            var chrNode = $("<span>").text(qchar);
            if (i >= atext.length) {
                chrNode.css('background', '#faa');
            }
            else if (qchar != achar) {
                chrNode.css('background', 'yellow');
            }
            this.$question.append(chrNode);
        }
    };
    ProblemView.prototype.updateEnterButton = function () {
        var yes = this.canEnter();
        this.$enterButton.removeClass(yes ? 'gray' : 'blue');
        this.$enterButton.addClass(yes ? 'blue' : 'gray');
    };
    ProblemView.prototype.canEnter = function () {
        var problem = this.model.getCurrentProblem();
        if (problem == null) {
            return false;
        }
        return problem.questionText.length == this.getAnswer().length;
    };
    ProblemView.prototype.getAnswer = function () {
        var rv = this.$answer.val().toString();
        // trim
        rv = rv.replace(/^\s+|\s+$/g, "");
        // 중복 공백을 하나의 공백으로
        var oldLength;
        do {
            oldLength = rv.length;
            rv = rv.replace(/  /g, ' ');
        } while (rv.length < oldLength);
        return rv;
    };
    ProblemView.prototype.resetAnswerText = function () {
        this.$answer.val('');
    };
    ProblemView.prototype.enableInput = function (yes) {
        this.$answer.prop('disabled', !yes);
        if (yes) {
            this.$answer.focus();
        }
    };
    ProblemView.prototype.show = function (yes) {
        if (yes) {
            this.$root.show();
        }
        else {
            this.$root.hide();
        }
    };
    return ProblemView;
}());
exports.ProblemView = ProblemView;
//# sourceMappingURL=ProblemView.js.map

/***/ }),

/***/ "./build/oe-anh-doe/src/ResultView.js":
/*!********************************************!*\
  !*** ./build/oe-anh-doe/src/ResultView.js ***!
  \********************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";

Object.defineProperty(exports, "__esModule", { value: true });
var ResultView = /** @class */ (function () {
    function ResultView(model) {
        var _this = this;
        this.model = model;
        this.$root = $('#resultDlg');
        var child = function (cname) { return $("#resultDlg " + cname); };
        this.$tbody = child('#tbody');
        this.$trYesTemplate = child('#trYes');
        this.$trNoTemplate = child('#trNo');
        this.$retryButton = child('#retryButton');
        this.$perfect = child('#perfect');
        this.$trYesTemplate.detach();
        this.$trNoTemplate.detach();
        this.$retryButton.click(function () {
            _this.onRetry();
        });
    }
    ResultView.prototype.onEnterKey = function () {
        this.onRetry();
    };
    ResultView.prototype.update = function () {
        this.$tbody.empty();
        for (var i in this.model.problems) {
            var p = this.model.problems[i];
            var c = this.model.retryCounts[i];
            var tr = void 0;
            if (c == 0) {
                tr = this.$trYesTemplate.clone(false, true);
            }
            else {
                tr = this.$trNoTemplate.clone(false, true);
                tr.find('#retryCount').text(c);
            }
            tr.find('#question').text(p.questionText);
            this.$tbody.append(tr);
        }
        if (this.model.wasPerfect()) {
            this.$retryButton.hide();
            this.$perfect.show();
        }
        else {
            this.$retryButton.show();
            this.$perfect.hide();
        }
    };
    ResultView.prototype.show = function (yes) {
        if (yes) {
            this.$root.show();
        }
        else {
            this.$root.hide();
        }
    };
    return ResultView;
}());
exports.ResultView = ResultView;
//# sourceMappingURL=ResultView.js.map

/***/ }),

/***/ "./build/oe-anh-doe/src/main.js":
/*!**************************************!*\
  !*** ./build/oe-anh-doe/src/main.js ***!
  \**************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";

Object.defineProperty(exports, "__esModule", { value: true });
var Model_1 = __webpack_require__(/*! ./Model */ "./build/oe-anh-doe/src/Model.js");
var ProblemView_1 = __webpack_require__(/*! ./ProblemView */ "./build/oe-anh-doe/src/ProblemView.js");
var ResultView_1 = __webpack_require__(/*! ./ResultView */ "./build/oe-anh-doe/src/ResultView.js");
var ModalDialog_1 = __webpack_require__(/*! ./ModalDialog */ "./build/oe-anh-doe/src/ModalDialog.js");
var ModalWindow_1 = __webpack_require__(/*! ./ModalWindow */ "./build/oe-anh-doe/src/ModalWindow.js");
var Generator_1 = __webpack_require__(/*! ./Generator */ "./build/oe-anh-doe/src/Generator.js");
var rawData = [
    '{벚|벗|벛|벋|벝|벘}{꽃|꼿|꽂|꽅|꼳|꽀} 가지를 {꼿꼿|꽂꽂|꽃꽃|꼳꼳|꽅꽅|꽀꽀}하게 {꽂|꼿|꽃|꼳|꽅|꽀}{았|앗}다.',
    '<이를 빼야|밥을 먹어야|게임을 해야|학교에 가야|히녹스를 잡아야|리퍼부터 죽여야> 하는{데|대} 말이{에|애}요.',
    '<짐승같이|괴물같이|좀비가|투명드래곤이> 울부{짖|짓|짗|짇|짙|짔}{었|엇}다.',
    '난 {했|햇}으니까 다음은 네 차{례|래|레}야.',
    '숲 한가운{데|대}{에|애} <마스터 소드가|연필이|삽이> {꽂|꼿|꽃|꽅|꼳|꽀}혀 {있|잇}{었|엇}{대|데}.',
    '거기 {갔|갓}다 온 기억이 안 나는{데|대}요.',
    '빙그{레|래} 웃으시더니 말{씀|슴}하{셨|셧}다.',
    '당장 <의자|벤치>{에|애} {앉아|안자}라.',
    '<그늘|나무 밑>{에|애} 가서 잠깐 쉴까?',
    '물고기가 펄{떡|덕}거{렸|렷}다.',
    '{옛|옜}날 <집 안|사람|서울|마을>의 모습',
    '글{씨|시}를 잘 {썼|썻}다.',
    '주말 오전에는 {게|께|개|깨}임을 하면 안 {되|돼}는 거 잘 알{잖|잔}아?',
    '내가 {왜|외} 그{래|레}야 {돼|되}?',
    '양치를 하지 {않|안}으면 너의 치아가 무사하지 못할 것이다!',
    '그러면 {안|않} {돼|되}!',
    '그걸 먹으면 안 {돼|되}!',
    '그러면 {안|않} {돼|되}요!',
    '그걸 먹으면 안 {되|돼}지.',
    '그러면 {안|않} {됩|됍}니다!',
    '그러면 {왜|외} {안|않} {돼|되}?',
    '그러지 {않|안}{았|앗}다.',
    '그러지 {않|안}기로 {했|햇}{잖|잔}아?',
    '그러지 {않|안}고는 살 수가 없{었|엇}어요.',
    '이제 너랑 같이 게임 {안|않} 할 거야!',
    '발 {밟|발|밥}지 {않|안}기!'
];
$(document).ready(function () {
    var problems = Generator_1.generateProblemList(rawData, 5);
    var model = new Model_1.Model(problems);
    var pview = new ProblemView_1.ProblemView(model);
    var rview = new ResultView_1.ResultView(model);
    var wstack = new ModalWindow_1.ModalWindowStack();
    showProblemView();
    //-------------------------------------------------------------------------
    function showProblemView() {
        model.goToStart();
        pview.setUpQuestion();
        pview.resetAnswerText();
        wstack.showAndPush(pview);
        pview.enableInput(true);
        pview.onEnter = function () {
            pview.enableInput(false);
            var p = model.getCurrentProblem();
            if (pview.getAnswer() === p.rightAnswer) {
                return showCorrectDlg();
            }
            else {
                return showWrongDlg(p.rightAnswer);
            }
        };
    }
    //-------------------------------------------------------------------------
    function showCorrectDlg() {
        var buttonCaption = model.getCurrentProblemNumber() >= model.getTotalProblemCount()
            ? '결과 확인하기 ⏎'
            : '다음 문제 ⏎';
        var dlg = new ModalDialog_1.ModalDialog('correctDlg', 'kf_popin 0.7s', buttonCaption);
        wstack.showAndPush(dlg);
        dlg.onClose = function () {
            wstack.hideAndPop(dlg);
            model.next();
            if (model.isEnded()) {
                wstack.hideAndPop(pview);
                showResultView();
            }
            else {
                pview.setUpQuestion();
                pview.resetAnswerText();
                pview.enableInput(true);
            }
        };
    }
    //-------------------------------------------------------------------------
    function showWrongDlg(rightAnswer) {
        var dlg = new ModalDialog_1.ModalDialog('wrongDlg', 'kf_drop 0.7s', '다시 해보기 ⏎');
        dlg.findChild('#rightAnswer').text(rightAnswer);
        wstack.showAndPush(dlg);
        dlg.onClose = function () {
            wstack.hideAndPop(dlg);
            model.retry();
            pview.enableInput(true);
        };
    }
    //-------------------------------------------------------------------------
    function showResultView() {
        rview.update();
        if (model.wasPerfect()) {
            rview.onRetry = function () { };
        }
        else {
            rview.onRetry = function () {
                wstack.hideAndPop(rview);
                showProblemView();
            };
        }
        wstack.showAndPush(rview);
    }
});
//# sourceMappingURL=main.js.map

/***/ })

/******/ });
//# sourceMappingURL=bundle.js.map