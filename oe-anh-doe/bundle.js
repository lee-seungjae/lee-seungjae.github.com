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
        this.currentProblemNumber = 1;
        this.totalProblemCount = problems.length;
    }
    Model.prototype.goToStart = function () {
        this.retryCounts = [];
        this.currentProblemNumber = 1;
        this.thisProblemRetryCount = 0;
    };
    Model.prototype.retry = function () {
        ++this.thisProblemRetryCount;
    };
    Model.prototype.next = function () {
        this.retryCounts.push(this.thisProblemRetryCount);
        this.thisProblemRetryCount = 0;
        ++this.currentProblemNumber;
    };
    Model.prototype.getCurrentProblem = function () {
        return this.problems[this.currentProblemNumber - 1];
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
        this.$answer.keypress(function (event) {
            if (event.originalEvent.keyCode === 13) {
                _this.onEnter();
                event.stopPropagation();
            }
        });
        this.$enterButton.click(function () {
            _this.onEnter();
        });
    }
    ProblemView.prototype.setUpQuestion = function () {
        this.$currentProblemNumber.text(this.model.currentProblemNumber);
        this.$totalProblemCount.text(this.model.totalProblemCount);
        this.$question.text(this.model.getCurrentProblem().questionText);
        this.$answer.val('').focus();
    };
    ProblemView.prototype.getAnswer = function () {
        return this.$answer.val().toString();
    };
    ProblemView.prototype.resetAnswerText = function () {
        this.$answer.val('');
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
    ResultView.prototype.update = function () {
        this.$tbody.empty();
        var retryCountSum = 0;
        for (var i in this.model.problems) {
            var p = this.model.problems[i];
            var c = this.model.retryCounts[i];
            retryCountSum += c;
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
        if (retryCountSum > 0) {
            this.$retryButton.show();
            this.$perfect.hide();
        }
        else {
            this.$retryButton.hide();
            this.$perfect.show();
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
var Generator_1 = __webpack_require__(/*! ./Generator */ "./build/oe-anh-doe/src/Generator.js");
var Model_1 = __webpack_require__(/*! ./Model */ "./build/oe-anh-doe/src/Model.js");
var ProblemView_1 = __webpack_require__(/*! ./ProblemView */ "./build/oe-anh-doe/src/ProblemView.js");
var ResultView_1 = __webpack_require__(/*! ./ResultView */ "./build/oe-anh-doe/src/ResultView.js");
// TODO:
// 문제에 같은/다른 텍스트 하이라이트 표시
var rawData = [
    '{벚|벗|벛|벋|벝|벘}{꽃|꼿|꽂|꽅|꼳|꽀} 가지를 {꼿꼿|꽂꽂|꽃꽃|꼳꼳|꽅꽅|꽀꽀}하게 {꽂|꼿|꽃|꼳|꽅|꽀}{았|앗}다',
    '<이를 빼야|밥을 먹어야|게임을 해야|학교에 가야|히녹스를 잡아야|리퍼부터 죽여야> 하는{데|대} 말이{에|애}요',
    '<짐승같이|괴물같이|좀비가|투명드래곤이> 울부{짖|짓|짗|짇|짙|짔}{었|엇}다',
    '난 {했|햇}으니까 다음은 네 차{례|래|레}야',
    '숲 한가운{데|대}{에|애} <마스터 소드가|연필이|삽이> {꽂|꼿|꽃|꽅|꼳|꽀}혀 {있|잇}{었|엇}{대|데}',
    '거기 {갔|갓}다 온 기억이 안 나는{데|대}요',
    '빙그{레|래} 웃으시더니 말{씀|슴}하{셨|셧}다',
    '당장 <의자|벤치>{에|애} {앉아|안자}라',
    '<그늘|나무 밑>{에|애} 가서 잠깐 쉴까?',
    '물고기가 펄{떡|덕}거{렸|렷}다',
    '{옛|옜}날 <집 안|사람|서울|마을>의 모습',
    '글{씨|시}를 잘 {썼|썻}다',
    '주말 오전에는 {게|께|개|깨}임을 하면 안 {되|돼}는 거 잘 알{잖|잔}아',
    '내가 {왜|외} 그{래|레}야 {돼|되}?',
    '양치를 하지 {않|안}으면 너의 치아가 무사하지 못할 것이다',
    '그러면 {안|않} {돼|되}!',
    '그걸 먹으면 안 {돼|되}',
    '그러면 {안|않} {돼|되}요!',
    '그걸 먹으면 안 {되|돼}지',
    '그러면 {안|않} {됩|됍}니다!',
    '그러면 {왜|외} {안|않} {돼|되}?',
    '그러지 {않|안}{았|앗}다',
    '그러지 {않|안}기로 {했|햇}{잖|잔}아',
    '그러지 {않|안}고는 살 수가 없{었|엇}어요',
    '이제 너랑 같이 게임 {안|않} 할 거야!'
];
$(document).ready(function () {
    //return testResultView();
    var problems = Generator_1.generateProblemList(rawData, 5);
    var model = new Model_1.Model(problems);
    var pview = new ProblemView_1.ProblemView(model);
    var rview = new ResultView_1.ResultView(model);
    function reset() {
        rview.show(false);
        model.goToStart();
        pview.setUpQuestion();
        pview.resetAnswerText();
        pview.show(true);
    }
    pview.onEnter = function () {
        var p = model.getCurrentProblem();
        if (pview.getAnswer() !== p.rightAnswer) {
            alert("\uD2C0\uB838\uC5B4\uC694.. \uD83D\uDE22\n\n\uC815\uB2F5\uC740 \"" + p.rightAnswer + "\" \uC785\uB2C8\uB2E4.\n\n\uB2E4\uC2DC \uD574\uBCFC\uAE4C\uC694?");
            model.retry();
            pview.resetAnswerText();
            return;
        }
        alert('맞았어요! 👏');
        model.next();
        if (model.getCurrentProblem()) {
            pview.setUpQuestion();
            pview.resetAnswerText();
        }
        else {
            pview.show(false);
            rview.update();
            rview.show(true);
        }
    };
    rview.onRetry = function () {
        reset();
    };
    // 초기화
    reset();
});
function testResultView() {
    var problems = [
        { questionText: '외않되', rightAnswer: '왜안돼' },
        { questionText: '시럽계', rightAnswer: '실업계' },
        { questionText: '사생활치매', rightAnswer: '사생활침해' }
    ];
    var model = new Model_1.Model(problems);
    var rview = new ResultView_1.ResultView(model);
    model.goToStart();
    model.next();
    model.retry();
    model.next();
    model.retry();
    model.retry();
    model.next();
    rview.update();
    rview.onRetry = function () { return alert('RETRY'); };
}
//# sourceMappingURL=main.js.map

/***/ })

/******/ });
//# sourceMappingURL=bundle.js.map