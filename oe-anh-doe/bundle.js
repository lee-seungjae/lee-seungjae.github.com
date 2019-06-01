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

/***/ "./build/oe-anh-doe/src/FeedbackDlg.js":
/*!*********************************************!*\
  !*** ./build/oe-anh-doe/src/FeedbackDlg.js ***!
  \*********************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";

Object.defineProperty(exports, "__esModule", { value: true });
var FeedbackDlg = /** @class */ (function () {
    function FeedbackDlg(nodeId, animationStyle) {
        var _this = this;
        this.animationStyle = animationStyle;
        this.$background = $('#modalBackground');
        this.$window = $("#modalBackground #" + nodeId);
        this.$closeButton = this.findChild('#closeButton');
        this.$closeButton.click(function (ev) { _this.endModal(); ev.preventDefault(); ev.stopPropagation(); });
    }
    FeedbackDlg.prototype.findChild = function (selector) {
        return this.$window.find(selector);
    };
    FeedbackDlg.prototype.doModal = function (buttonCaption) {
        var _this = this;
        return new Promise(function (resolve, reject) {
            console.assert(_this.resolve === undefined);
            _this.resolve = resolve;
            // UI 이벤트에서 에러핸들링하기가 마땅찮아서
            // reject는 쓰지 않는다
            _this.$closeButton.text(buttonCaption);
            _this.$background.show();
            _this.$window.css('animation', _this.animationStyle);
            _this.$window.show();
            _this.$closeButton.focus();
        });
    };
    FeedbackDlg.prototype.endModal = function () {
        this.$background.hide();
        this.$window.hide();
        console.assert(this.resolve !== undefined);
        var resolve = this.resolve;
        this.resolve = undefined;
        resolve();
    };
    return FeedbackDlg;
}());
exports.FeedbackDlg = FeedbackDlg;
//# sourceMappingURL=FeedbackDlg.js.map

/***/ }),

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
        this.$answer.keypress(function (event) {
            if (event.keyCode === 13) {
                event.stopPropagation();
                event.preventDefault();
                _this.onEnterKey();
            }
        });
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
        this.updateEnterButton();
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

/***/ "./build/oe-anh-doe/src/ResultDlg.js":
/*!*******************************************!*\
  !*** ./build/oe-anh-doe/src/ResultDlg.js ***!
  \*******************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";

Object.defineProperty(exports, "__esModule", { value: true });
var ResultDlg = /** @class */ (function () {
    function ResultDlg(model) {
        var _this = this;
        this.model = model;
        this.$root = $('#resultDlg');
        this.$tbody = this.$root.find('#tbody');
        this.$trYesTemplate = this.$root.find('#trYes');
        this.$trNoTemplate = this.$root.find('#trNo');
        this.$retryButton = this.$root.find('#retryButton');
        this.$perfect = this.$root.find('#perfect');
        this.$trYesTemplate.detach();
        this.$trNoTemplate.detach();
        this.$retryButton.click(function () { return _this.endModal(); });
    }
    ResultDlg.prototype.doModal = function () {
        var _this = this;
        return new Promise(function (resolve, reject) {
            console.assert(_this.resolve === undefined);
            _this.resolve = resolve;
            // UI 이벤트에서 에러핸들링하기가 마땅찮아서
            // reject는 쓰지 않는다
            _this.update();
            _this.$root.show();
            _this.$retryButton.focus();
        });
    };
    ResultDlg.prototype.endModal = function () {
        this.$root.hide();
        console.assert(this.resolve !== undefined);
        var resolve = this.resolve;
        this.resolve = undefined;
        resolve();
    };
    ResultDlg.prototype.update = function () {
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
    return ResultDlg;
}());
exports.ResultDlg = ResultDlg;
//# sourceMappingURL=ResultDlg.js.map

/***/ }),

/***/ "./build/oe-anh-doe/src/main.js":
/*!**************************************!*\
  !*** ./build/oe-anh-doe/src/main.js ***!
  \**************************************/
/*! no static exports found */
/***/ (function(module, exports, __webpack_require__) {

"use strict";

var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : new P(function (resolve) { resolve(result.value); }).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __generator = (this && this.__generator) || function (thisArg, body) {
    var _ = { label: 0, sent: function() { if (t[0] & 1) throw t[1]; return t[1]; }, trys: [], ops: [] }, f, y, t, g;
    return g = { next: verb(0), "throw": verb(1), "return": verb(2) }, typeof Symbol === "function" && (g[Symbol.iterator] = function() { return this; }), g;
    function verb(n) { return function (v) { return step([n, v]); }; }
    function step(op) {
        if (f) throw new TypeError("Generator is already executing.");
        while (_) try {
            if (f = 1, y && (t = op[0] & 2 ? y["return"] : op[0] ? y["throw"] || ((t = y["return"]) && t.call(y), 0) : y.next) && !(t = t.call(y, op[1])).done) return t;
            if (y = 0, t) op = [op[0] & 2, t.value];
            switch (op[0]) {
                case 0: case 1: t = op; break;
                case 4: _.label++; return { value: op[1], done: false };
                case 5: _.label++; y = op[1]; op = [0]; continue;
                case 7: op = _.ops.pop(); _.trys.pop(); continue;
                default:
                    if (!(t = _.trys, t = t.length > 0 && t[t.length - 1]) && (op[0] === 6 || op[0] === 2)) { _ = 0; continue; }
                    if (op[0] === 3 && (!t || (op[1] > t[0] && op[1] < t[3]))) { _.label = op[1]; break; }
                    if (op[0] === 6 && _.label < t[1]) { _.label = t[1]; t = op; break; }
                    if (t && _.label < t[2]) { _.label = t[2]; _.ops.push(op); break; }
                    if (t[2]) _.ops.pop();
                    _.trys.pop(); continue;
            }
            op = body.call(thisArg, _);
        } catch (e) { op = [6, e]; y = 0; } finally { f = t = 0; }
        if (op[0] & 5) throw op[1]; return { value: op[0] ? op[1] : void 0, done: true };
    }
};
var _this = this;
Object.defineProperty(exports, "__esModule", { value: true });
var Model_1 = __webpack_require__(/*! ./Model */ "./build/oe-anh-doe/src/Model.js");
var ProblemView_1 = __webpack_require__(/*! ./ProblemView */ "./build/oe-anh-doe/src/ProblemView.js");
var ResultDlg_1 = __webpack_require__(/*! ./ResultDlg */ "./build/oe-anh-doe/src/ResultDlg.js");
var FeedbackDlg_1 = __webpack_require__(/*! ./FeedbackDlg */ "./build/oe-anh-doe/src/FeedbackDlg.js");
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
    '발 {밟|발|밥}지 {않|안}기!',
    '마치 지옥에 {갔다|갓다} 온 {것|겄} {같아|가타}',
    '치과에 가서 이를 {빼야|뻬야} {해|헤}요',
    '양말을 {벗|벘}{겼|겻}다',
    '제 몸{에|애} 맞{네|내}요',
    '우연{히|이} 만{났|낫}어요',
    '우리 집 마당{에|애} {있|잇}는 {텃|턷}{밭|받}',
    '숲 속으로 함{께|깨} 떠나요',
    '어느새 깜깜한 밤인{데|대}',
    '그 친구 파라 장인이라던{데|대}',
    '같이 술{래|레}잡기 할{래|레}?',
    '무{릎|릅}이 너무 아{팠|팟}고',
    '울음을 터뜨릴 것 같{았|엤}던',
];
$(document).ready(function () { return __awaiter(_this, void 0, void 0, function () {
    //-------------------------------------------------------------------------
    function solveSingleProblem(p) {
        return __awaiter(this, void 0, void 0, function () {
            var caption;
            return __generator(this, function (_a) {
                switch (_a.label) {
                    case 0:
                        problemView.setUpQuestion();
                        problemView.resetAnswerText();
                        _a.label = 1;
                    case 1:
                        if (false) {}
                        problemView.enableInput(true);
                        return [4 /*yield*/, waitForEnter()];
                    case 2:
                        _a.sent();
                        problemView.enableInput(false);
                        if (!(problemView.getAnswer() !== p.rightAnswer)) return [3 /*break*/, 4];
                        wrongDlg.findChild('#rightAnswer').text(p.rightAnswer);
                        return [4 /*yield*/, wrongDlg.doModal('다시 해보기 ⏎')];
                    case 3:
                        _a.sent();
                        model.retry();
                        return [3 /*break*/, 6];
                    case 4:
                        caption = (model.getCurrentProblemNumber() >= model.getTotalProblemCount())
                            ? '결과 확인하기 ⏎'
                            : '다음 문제 ⏎';
                        return [4 /*yield*/, correctDlg.doModal(caption)];
                    case 5: return [2 /*return*/, _a.sent()];
                    case 6: return [3 /*break*/, 1];
                    case 7: return [2 /*return*/];
                }
            });
        });
    }
    //-------------------------------------------------------------------------
    function waitForEnter() {
        return __awaiter(this, void 0, void 0, function () {
            return __generator(this, function (_a) {
                return [2 /*return*/, new Promise(function (resolve, reject) {
                        problemView.onEnter = function () { return resolve(); };
                    })];
            });
        });
    }
    var problems, model, problemView, resultDlg, correctDlg, wrongDlg;
    return __generator(this, function (_a) {
        switch (_a.label) {
            case 0:
                problems = Generator_1.generateProblemList(rawData, 5);
                model = new Model_1.Model(problems);
                problemView = new ProblemView_1.ProblemView(model);
                resultDlg = new ResultDlg_1.ResultDlg(model);
                correctDlg = new FeedbackDlg_1.FeedbackDlg('correctDlg', 'kf_popin 0.7s');
                wrongDlg = new FeedbackDlg_1.FeedbackDlg('wrongDlg', 'kf_drop 0.7s');
                _a.label = 1;
            case 1:
                if (false) {}
                model.goToStart();
                problemView.show(true);
                _a.label = 2;
            case 2:
                if (!!model.isEnded()) return [3 /*break*/, 4];
                return [4 /*yield*/, solveSingleProblem(model.getCurrentProblem())];
            case 3:
                _a.sent();
                model.next();
                return [3 /*break*/, 2];
            case 4:
                problemView.show(false);
                return [4 /*yield*/, resultDlg.doModal()];
            case 5:
                _a.sent();
                return [3 /*break*/, 1];
            case 6: return [2 /*return*/];
        }
    });
}); });
//# sourceMappingURL=main.js.map

/***/ })

/******/ });
//# sourceMappingURL=bundle.js.map