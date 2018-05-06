/*
 * FeatureCompareVeracityTest.cpp
 *
 *  Created on: 2016年1月7日
 *      Author: hadoop
 */

#include <iostream>
#include <regex>
#include <locale>
#include "feature_compare.h"
#include "common_utilits.h"
#include "sample_log.h"
using namespace gdface;
using namespace std;

inline face_code to_face_code(const char*hex){
	return *reinterpret_cast<face_code*>(hex_to_bytes(hex).data());
}
inline face_code to_face_code(const string &hex){
	return *reinterpret_cast<face_code*>(hex_to_bytes(hex).data());
}

int main() {
//	setlocale(LC_CTYPE, "");
	//std::locale::global(std::locale(""));

	//locale lc("zh_CN.UTF-8");

	//std::wcout.imbue(std::locale(std::locale(), "", LC_CTYPE));
	//std::cout.imbue(std::locale(std::locale(), "", LC_CTYPE));

	const wchar_t * p = L"[char pointer汉字]";
	SAMPLE_OUT("{}TEST宽字符集输出测试{} {}", "hello",0.7,p);

	std::wcout << gdface::tolower(std::wstring(L"字符串转小写TEST HELLO WORD 测试")) << std::endl;
	std::wcout << gdface::toupper(std::wstring(L"字符串转大写test hello word 测试")) << std::endl;

	std::wcout << to_wide_string("string到wstring转换测试") << std::endl;
	std::cout << to_byte_string(L"wstring到string转换测试") << std::endl;

	auto g_result1 = split(std::string("hello,do you ;know the word?"), std::string("[\\s,;?]+"));
	std::copy(g_result1.begin(), g_result1.end(), std::ostream_iterator<std::string>(std::cout, "<->"));

	std::cout << "=============" << std::endl;

	auto g_result2 = split(std::wstring(L"lao ban 老板，来份，小龙虾，快点啊！？"), std::wstring(L"[\\s，？！]+"));
	std::copy(g_result2.begin(), g_result2.end(), std::ostream_iterator<std::wstring, std::wstring::value_type>(std::wcout, L"<->"));

	std::cout << "=============" << std::endl;

	auto s_result = split("{}hello{}world{}", "\\{\\}");
	std::copy(s_result.begin(), s_result.end(), std::ostream_iterator<std::string>(std::cout, "<->"));
	std::cout << "=============" << std::endl;
	auto c_result = split("hello,do you ;know the word?", "[\\s,;?]+");
	std::copy(c_result.begin(), c_result.end(), std::ostream_iterator<std::string>(std::cout, "<->"));
	std::cout << "=============" << std::endl;

	auto ws_result = split(L"lao ban 老板，来份，小龙虾，快点啊！？", L"[\\s，？！]+");
	std::copy(ws_result.begin(), ws_result.end(), std::ostream_iterator<std::wstring, std::wstring::value_type>(std::wcout, L"<->"));

	//const char *hex_code1 = "f48cb1bf7d0958c0dcdb3a40ac589140bf0b813e34f45740fca3adbf4c3dd6bd2837154091b8b2bf9a4443bf6768b73f99ec483fa312873e6c7ea4bf458290bfef41cebe6bcb1f3e7b40a5bec8847ebfd091f9bf67a344bfc68343bd8e6b2f3e8aa7953f3c160d3f258c76bff0c41dbe6c327fbfa06c53bf93e8fabdc8221bbfb657d9bfef57183e47ca4ebcdff91e3e192c9ebf1b21813fdfeb95bf2e5a523e8557483e72442dbfb593a43e800c5dbe247609bfc8b5a1be728aac3ff319a83e34d959bfd92c933e47f7b5bd6c0b8dbf0229403f022b3a3e069d8f3f2a1513be6fb30e3f89d822bed9719f3f6370bfbe1d1b32be92db10bf7288233f8bf14d3f549f21bfa683983e0440c0bf5eb0323f0654b2bdbb13e7bfb02dc43d7bffe6be600e223fac2e18bf4031b4bf6409c73d8de28dbd2f519dbe51cac83f66328cbe48a4a13ed9d96bbf7d885cbf9f63c53eddf5763fb815583e0333573f49dc3d3e1d7566bfc021b83ed2506dbed43fe6bffb684a3e0c89933f9efebfbe25433bbf7230ef3db4d7b93f868539be8c8d883e90c265bceb83173ea5b9bbbfa43a6d3d0ab403be240b05bf3de240bf95cbb43d1c180d3e1b74b6be4df0d5beb831913e939c2c3e9eaa8e3eb87a423df73d3e3cea20e6bc0fad1ebf2fc45f3fa4c9a3be5658c93f0e3687bf8659913dc50e0d3fa335b03e91b7833e56d5ad3e054f283f563c493f0ce34a3f860367bfde6e813e73b2573f0c7b25be7683a9be4ea4cf3ec659243f995874bf6976043eaae9363f17b9263ffb7e8b3d133617be4873893e803bdebe270f35bf8a0d35bec542ad3e534b7e3f342636bf4555723fbb1f92bdddb09fbf5cc2d93dbb60603d9585713f2f5876be67e006becc6d53bfec4d3bbd76ac2dbf5f66a5bda73e93bf7e59933ef055f33e40981e3f95a977bea843913dbbc38fbe3d8095bff338b0be95edad3e5fb3dabed54041bc787b8b3ea396a43f00bc61bf4103dfbea39f08beaa3c413ef1876abf598c24bfea710fbf5daeb5be70ac12bff5cbbcbe8119f33e8cb7273f6c05dfbedd454fbf8c6d6a3ff346033fde1fc23d3a7dd93e0c73c33effa2263f197070bd94703cbecadb803eee316ebebcc167bf62c49dbed17e04bf0ca316be0a00cabd4633863d1bc700bfc2a42bbf77d5a83d783a63bd00e63f1e53a86340";
	//const char *hex_code2 = "f9b10fc00712f4c000c31d405a5ba240d77c0bc0d6ed533f71f680c00048f03f855703c0653b1d3fcd2acdbfc11f6c3f10b0c7bfb7f2b1bfdcaa8abf068602c03dee02bf2de7b1be0ad18bbf6a39e0bf94a1bc3e671664c06884b7bffbb4e73f8b24a6bfdfe9e53d94e2d93e48b462402db4ba3fd6a1c63e4fab41bf750e10c01cb02840218bf53f1f43a2bfc9ea10bf53d960bfa6666e3f4f3c163ffc99623f908f42c059a719403817c1bfddc209c022b03d40cc2f0f3dc5b3b5bedb83cb3e695615c0368f873e068ebdbfe1fa37bf5fc261bf1b613540366227c08595f93d463682bf40f0833dad1ba7bf064f40c0106317bfcbd28dbf0b2c0c40925d7a3fb5be4640ae231340645f5fc054e4863f1c80febf064981bf13b5e6bfc3120cc0643a9fbfac3e9dbfb1a8c2be44293ac0b50ffdbf6eb9e93e214c67bf0d6f05405576b3bd0ad3154077521ebe74edb43f58209e3f1af0983ffe6d8abefd16d9bf8d299bbdd528294056d2ca3e518f95bf6f162e3f13606bbf5539b13f861f5bbf1b90ab3d7d29af3f1dd0e7beef210a3f2a2ce4bf92001440c490413f7d6629bf84cc53be327775bf4f8a683f23c14e3fab2fbdbfc70620bf5b00b1be9cb0e93fc9a654bd11751c3f595e10403ef8eb3e81678abf98a9373f3e228f3f29019e3f86a002c0cac3043fe0ae8abf0446c8bfddddc93fd2ea02bffa23b0bdc82ba2bf41328cbf9b4b8ebe6fa7494016ebfc3e879237bf7583ddbec40675bf8ccb0fbf958b1abf31cd0240d5ececbe3fa82c3f6083933f328223bfe17ad13ec6d2ff3f712ba63df7b16dc075698fbfbbe8913d06f2ed3fc8fe3c3f41e548bfce130dbfd44fafbd648eafbe41ca043de88b2e3f4148a7bff3c0b9bf4228413f6af98cbf182fddbebe53c93cdb6a0bbf134bbabe697934bf4965913e4e731bbf4606e03e973da33e77b0c43ead6d51bfe1d8bb3eba211ebf77bd0740446c373e12aaeebe3b6e9b3f6c83d0bfc07d90bfa99d613f763be2bfab35a3be5e23e13e91ecdabef2ea1340a2720d3f0062333ef28fb23dd70582bd2cce423f9d438ebe1daba0bfc375cc3e1ec61fc099391fbe0594bb3f85aba0bd51bd1ac001c21340a08890bf2e01513ebd4a953fcc8995bf2269823f839c0e40c4f86abfe97f7a3f196804bf830d4ac0ae7b54bf001876e198518040";
	//const char * hex_code1="4a8900c0619d38c089f2803fcf813840daa3a73f65f9713f58732c3fcf2a4a40ada30840a85d843ec746b4bff985a03f59285bbf98bce73f2a1803c0553c883fc7c7b4bf2b6e563f0f4693bf5e6156bf77c555bff110b9bf5bf786bf2e14c7bc5f881ebeedbfa83e098ca8bf49e0a0bf641ea03f5876bc3f9c3a523ea54600c0564aa4bde6952bbf364a653ece620fc0a1b9b1bfe38152bfe7c0213f16a558bf1dc0e53efb3915bf0c9816c053d3b5bdd9646fbf4e8f32bf4f1d17be804851bfc88e083ff8eb21bd7912babff9d31dbfcbe89e3fefc60f3f9b5b0b3fb8fa963f607f57bfca7a36bf1032753edf6689be989a0c3fa49ca83d302c50be65461bc0c476afbee56c043fa6ea49bef548363f6d7b06bfbd9b3bbe74f3553ec70edcbecc46e33fb1e76e3fd193cbbfb0c3b63f8661343f49823ebfb778353ec958393ee1b735be646d44bfbd0ec53eab4e5d3f0870f33d079477bf0a09c43ea33b78be515ffc3fb340b0bf88b304bd642c1abdfb17633eba1417bed455d7bf7dd1813f1550a6bf473c03bf9187663f2b68c2bf51138e3e09a128bf3b41bf3c01b58e3f81af81bef08ddebfe9f7ce3e2663183f854d48be4529debf2bfaf63e30d04cbfd410833f57f1473f3b6a083f6a93583f09f63fbfe49d1c3e7cce3ebc6353bcbec7bae53ee88b49bfbe56d6be3b03493f7ac1863ed380e1be785047bf91e7b63e47457cbfbf3eb83f457124bf1c978dbfd0ffd13dbe169bbfe2b981bff9a4593dea91a7befb1cbabfdd079d3e0fe5f13e347926bfc5c3293f554fb2bff73765bf65d5403f0966a63fa62d963fbb820240959cb33fb2b9963e6d9b0fbeda95a53f0dacda3f31af0d3ec4ca913fca7a86bfaeb6373eef29debd87f06b3f1329123fbf838a3f950b0c3f4f65823fdb3b5dbf8ed01bbfe08fdfbe2f6c24bfbef913be56de703f8d0f563f916df03fb9fe35be475dca3fea45a33f40d0f03fc1f95d3ff806803ecf09a93f8cc80f3cff3567bf95460dbf1b44923e61555b3e130fe8be9150193f2a89913fc4ed4b3eb2eb2fbf611205bedea9bbbf5a4fac3eb0c2edbf957dcfbee3b22ebfeebc70bf30f3383f622a103f5d2830be9f36f23e07332dbc1b8ca7be2a3755bee07695bf50f68d3f56f936bd5e472d3fd74419bd194386bfac25bcbea479bf3d00a4b8b5c7d76a40";
	//const char * hex_code2="63923fc0837ecdc0c1d5293fd47a2c4000fa933f6ba60340e8910bbeaeac773fe3602e40069b493f16f541be26042e40e69f02c073d522c01258e3bf6048b6bf1086613e9ac1c4be12fbb1bff9f440bf7e7e10c0599bffbfda7abbbf18a7b93f3a45d0bfacfd343e5d453cbfdab5e6bc1f2ea93e8f31bd3f4badc1bedc1a26c002d1fb3f42c837bfb9f5db3f284b9d3f38662abff25c973e008dc0bd9dcaa9bf203db33e331f8e3f95110fc00c7f92bf6dc9533edc724cbf79d7c9bf0c53c6be4f2baabfeaf15fbf3d2fb83e10fa273f1c88e5bee559993f85eda9bfae39d33f5deaba3fa41310be4c5e5b3fc5e657bfa99e2fbfbb2bf63fb823f63f7d4e64bf54e9c13c086d04403f69cebf1256f33ec1aa8ebf1a6227c05e54093e1a0292baebc4f13fed3b7b3f363a97bf0e74123fdf27f9beceffd7bf917ae3bec449673ec16dde3f7c5bc23edcfb653f6787b63e3d55943e93c01b40b36e0cbe7d2df63d77dcdf3f0ba821bebc37823eab73523e7281073f2d439ebeb7e1e03ec6086c3f273900c061996b3fe2bc683ff35265bf93b4bdbea496b23a9bae1a3f145010403498903e01ca8fbfb74a9e3f43e4a63f5ab319bfe3e21fbf00351f3f97d5453f5bd199bedb397a3e92db5a3f082805402bf8a53ed75c6b3f30483b3f951c783f340776bf2e7889be67309bbfb3f4863eca8a5f3fc99e2b3ef90187befc5e37be380e62bfdb4e863fbd1033bf8e5cc8bf5a39213fe72a21bf96d15cbf3f2793bff6064c3e25c681bfeefa31bf6a9eb33f5c80c3bfb75ce6be4de589bf8487fc3d179c0bbfc17913bf5c0fd93e6bb940bf89a80a3fd9403f3f2ae12dbf304c0e3f03f968bfd3735ebd6e5e743db0e4b6be5169993edbfcd3bf4983253fb409043fe1ef00403b7db43f8c8013bf471422bf597997bf18db1d3f2f17c4bf8b600e3ff71d36bf4b5bbf3efcab3e3ff56710bfe1f51a4022b7a33e3accb73fe7bb2c3fec746dbf1b99703d3d6383bfc0bab53e97d860bf39812a3f2411933d73aa793f3d5b05bfc21f98beecf8c6be5bd2e93e45d71cbf9d6d693fbf398ebe997129c08e28d9be57ff08be80f893bfa68cdc3e730cd63f42e41ebfe60e73bfa230ab3efd65283f6fbebd3e277ccabe9af087be295229bf0ea6453fcc036d3f119b0dbf8f655dbf59961cbfe848944480f97240";

	//try {
	//	//cout << src1 << endl;
	//	//auto bytes1 = hex_to_bytes(src1);
	//	auto code1 =to_face_code(hex_code1);
	//	auto code2 =to_face_code(hex_code2);
	//	auto sim=feature::compare(code1,code2);
	//	cout<<"sim="<<sim<<endl;
	//}
	//catch (exception &e) {
	//	cout << e.what() << endl;
	//}
	//return 0;
}

