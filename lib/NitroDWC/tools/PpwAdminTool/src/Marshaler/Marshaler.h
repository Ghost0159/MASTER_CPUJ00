// Marshaler.h

#pragma once

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;
#pragma warning( disable:4691 )

using namespace std;

#pragma pack(push, 4)
#include "WFLBY_USER_PROFILE.h"
#include "lobby.h"
#pragma pack(pop)

namespace AdminTool {

    template<class T>
    array<Byte>^ NativeTypeToCliByteArray(const T& native)
    {
        array<Byte>^ arr = gcnew array<Byte>(sizeof(native));
        // native�ւ͏������܂Ȃ��̂�const�O��
        Marshal::Copy(IntPtr(const_cast<T*>(&native)), arr, 0, sizeof(native)); 
        return arr;
    }

    template<class T>
    vector<T> ListToNativeArray(List<T>^ list)
    {
        vector<T> v;
        if(list->Count == 0)
        {
            return v;
        }
        array<T>^ arr = list->ToArray();
        pin_ptr<T> nativeName = &arr[0];
        v.assign((T*)nativeName, (T*)(nativeName + arr->Length));
        return v;
    }
	
    template<class TargetT, class SourceT>
    vector<TargetT> ListToNativeArray(List<SourceT^>^ list)
    {
        vector<TargetT> v;
		for each(SourceT^ elem in list)
		{
			v.push_back(elem->ToNative());
		}
        return v;
    }

	// �l�C�e�B�u�Ȕz�񂩂�l�^�̃��X�g�ւ̕ϊ�
    template<class TargetT, class SourceT>
    List<TargetT>^ NativeArrayToList(const SourceT* ary, int num)
    {
        // �ǂ����@��������Ȃ������̂ň���ǉ�
        List<TargetT>^ list = gcnew List<TargetT>();
        for(int i=0; i<num; i++)
        {
            list->Add(TargetT(ary[i]));
        }
        return list;
    }

	// �l�C�e�B�u�Ȕz�񂩂�n���h���̃��X�g�ւ̕ϊ�
    template<class TargetT, class SourceT>
    List<TargetT^>^ NativeArrayToManageList(const SourceT* ary, int num)
    {
        // �ǂ����@��������Ȃ������̂ň���ǉ�
        List<TargetT^>^ list = gcnew List<TargetT^>();
        for(int i=0; i<num; i++)
        {
            list->Add(gcnew TargetT(ary[i]));
        }
        return list;
    }

	// �z��̗v�f��A������
	template<class T>
	String^ Join(List<T>^ list, String^ token)
	{
		String^ result = "";
		for(int i=0; i<list->Count; i++)
		{
			if(i != 0)
			{
				result += token;
			}
			result += list[i];
		}
		return result;
	}

	// �z��̗v�f�������w�肵�ĘA������
	template<class T>
	String^ Join(List<T>^ list, String^ token, int num)
	{
		String^ result = "";
		for(int i=0; i<list->Count && i<num; i++)
		{
			if(i != 0)
			{
				result += token;
			}
			result += list[i];
		}
		return result;
	}

    // WFLBY_USER_PROFILE�̃}�l�[�W�\��
    // ����Ȃ��p�����[�^�͊O��
    public ref struct REF_WFLBY_USER_PROFILE
    {
        s32				userid;						// ���r�[�����[�UID
        u32				trainerid;					// �g���[�i�[ID					
        List<STRCODE>^	name;	                    // ���[�U��
        u8				sex;						// ����
        u8				region_code;				// ����R�[�h LANG_JAPAN�ȂǂȂ�
        u16				tr_type;					// �g���[�i�̌�����
        u16				nation;						// ���R�[�h
        u8				area;						// �n��R�[�h
        u8				rom_code;					// �����o�[�W����	VERSION_PLATINUM�Ƃ�

        REF_WFLBY_USER_PROFILE(s32 userId_, u32 trainerid_, List<STRCODE>^ name_, u8 sex_, u8 region_code_, u16 tr_type_, u16 nation_, u8 area_, u8 rom_code_)
        {
            userid = userId_;
            trainerid = trainerid_;
            name = name_;
            sex = sex_;
            region_code = region_code_;
            tr_type = tr_type_;
            nation = nation_;
            area = area_;
            rom_code = rom_code_;
        }
        
        REF_WFLBY_USER_PROFILE(IntPtr ptr, int size)
        {
            WFLBY_USER_PROFILE nativeProfile;
            if(size != sizeof(nativeProfile))
            {
                throw gcnew System::Exception("�T�C�Y����v���܂���");
            }
            memcpy(&nativeProfile, (void*)ptr, sizeof(nativeProfile));

            userid = nativeProfile.userid;
            trainerid = nativeProfile.trainerid;
            name = NativeArrayToList<STRCODE, STRCODE>(nativeProfile.name, PERSON_NAME_SIZE + EOM_SIZE);
            sex = nativeProfile.sex;
            region_code = nativeProfile.region_code;
            tr_type = nativeProfile.tr_type;
            nation = nativeProfile.nation;
            area = nativeProfile.area;
            rom_code = nativeProfile.rom_code;
        }
/*
        REF_WFLBY_USER_PROFILE(List<Byte>^ bytes)
        {
            WFLBY_USER_PROFILE* nativeProfile = (WFLBY_USER_PROFILE*)DWCi_GetVectorBuffer(ListToNativeArray(bytes));

            userid = nativeProfile->userid;
            trainerid = nativeProfile->trainerid;
            name = NativeArrayToList(nativeProfile->name, PERSON_NAME_SIZE + EOM_SIZE);
            sex = nativeProfile->sex;
            region_code = nativeProfile->region_code;
            tr_type = nativeProfile->tr_type;
            nation = nativeProfile->nation;
            area = nativeProfile->area;
            rom_code = nativeProfile->rom_code;
        }
*/
        array<Byte>^ ToByteArray()
        {
            vector<STRCODE> nativeName = ListToNativeArray(name);
            if(nativeName.empty())
            {
                throw gcnew System::Exception("���O����ł��B");
            }

            WFLBY_USER_PROFILE userProfile = {
                userid, // ���[�UID
                trainerid,   // �g���[�i�[ID
                {0},   // ���[�U���A��ő��
                0,      // ��������
                0,      // �����̍���GMT����
                {1, 2, 3, 4, 5, 6},    // �莝���̃|�P����
                {0},    // �|�P�����̃t�H�����f�[�^
                {0},    // ���t���O
                sex,      // ����
                region_code,      // ����R�[�h
                tr_type,      // �g���[�i�[�^�C�v
                nation,    // ���R�[�h
                area,      // �n��R�[�h
                0,      // �S���}�ӕێ��t���O
                0,      // �Q�[���N���A�t���O
                0,      // �^�b�`�g�C
                rom_code,     // ROM�o�[�W����
                0,      // �v���C���[�X�e�[�^�X
                0,      // �`�����n�߂�����
                {0},    // �Ō�ɍs��������
                {0}     // �Ō�ɍs�������ƂɊ֘A����l
            };
            memcpy(&userProfile.name, DWCi_GetVectorBuffer(nativeName),
                sizeof(userProfile.name[0]) * std::min(nativeName.size(), (std::size_t)(PERSON_NAME_SIZE + EOM_SIZE)));

            return NativeTypeToCliByteArray(userProfile);
        }
    };

	bool IsFreeQuestion(int questionNo)
	{
		return questionNo >= PPW_LOBBY_FREE_QUESTION_START_NO && questionNo < PPW_LOBBY_FREE_QUESTION_START_NO + PPW_LOBBY_FREE_QUESTION_NUM;
	}

	/// �A���P�[�g���e
	public ref struct REF_PPW_LobbyQuestionnaireRecord
	{
		s32 questionSerialNo;                   ///< ����ʂ��ԍ��B0����X�^�[�g�B
		s32 questionNo;                         ///< ����ԍ��BROM������:0�`59 �C�ӎ���:PPW_LOBBY_FREE_QUESTION_START_NO�`PPW_LOBBY_FREE_QUESTION_START_NO+PPW_LOBBY_FREE_QUESTION_NUM
		List<u16>^ questionSentence;   ///< �C�ӎ���B����ԍ����C�ӎ���͈̔͂������Ƃ��̂݊i�[����܂��B
		List<List<u16>^>^ answer;    ///< �C�ӎ���̉񓚁B����ԍ����C�ӎ���͈̔͂������Ƃ��̂݊i�[����܂��B
		List<u8>^ multiLanguageSummarizeFlags;     ///< �W�v����
		s32 isSpecialWeek;

		REF_PPW_LobbyQuestionnaireRecord(const PPW_LobbyQuestionnaireRecord& native)
		{
			questionSerialNo = native.questionSerialNo;
			questionNo = native.questionNo;
			questionSentence = NativeArrayToList<u16, u16>(native.questionSentence, PPW_LOBBY_MAX_QUESTION_SENTENCE_LENGTH);
			answer = gcnew List<List<u16>^>();
			for(int i=0; i<PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM; i++)
			{
				answer->Add(NativeArrayToList<u16, u16>(native.answer[i], PPW_LOBBY_MAX_ANSWER_LENGTH));
			}
			multiLanguageSummarizeFlags = NativeArrayToList<u8, u8>(native.multiLanguageSummarizeFlags, 12);
			isSpecialWeek = native.isSpecialWeek;
		}

		virtual String^ ToString() new
		{
			List<String^>^ answerStrs = gcnew List<String^>();
			for each(List<u16>^ ans in answer)
			{
				answerStrs->Add(IsFreeQuestion(questionNo) ? Util::UShortListToUnicodeString(ans) : "<�Ȃ�>");
			}

			return "����ʂ��ԍ��F" + questionSerialNo + Environment::NewLine +
				"����ԍ��F" + questionNo + Environment::NewLine +
				"���╶�F" + (IsFreeQuestion(questionNo) ? Util::UShortListToUnicodeString(questionSentence) : "<�Ȃ�>") + Environment::NewLine +
				"�񓚁F" + Join(answerStrs, ", ") + Environment::NewLine +
				"�W�v����F" + Join(multiLanguageSummarizeFlags, ", ", LANGUAGE_MAX) + Environment::NewLine +
				"�X�y�V�����E�B�[�N�F" + (isSpecialWeek ? "�͂�" : "������");
		}
	};

	/// �A���P�[�g���
	public ref struct REF_PPW_LobbyQuestionnaire
	{
		REF_PPW_LobbyQuestionnaireRecord^ currentQuestionnaireRecord;    ///< ���݂̃A���P�[�g���
		REF_PPW_LobbyQuestionnaireRecord^ lastQuestionnaireRecord;       ///< �O��̃A���P�[�g���
		List<s32>^ lastResult;         ///< �O��̎����̌���̌���
		List<s32>^ lastMultiResult;    ///< �O��̕����W�v����ł̏W�v���ʁB::lastQuestionnaireRecord�\���̂�multiLanguageSummarizeFlags�����o�ŕ\����錾��ł̏W�v���ʂł��B

        REF_PPW_LobbyQuestionnaire(IntPtr ptr, int size)
        {
            PPW_LobbyQuestionnaire native;
            if(size != sizeof(native))
            {
                throw gcnew System::Exception("�T�C�Y����v���܂���");
            }
            memcpy(&native, (void*)ptr, sizeof(native));

			currentQuestionnaireRecord = gcnew REF_PPW_LobbyQuestionnaireRecord(native.currentQuestionnaireRecord);
			lastQuestionnaireRecord = gcnew REF_PPW_LobbyQuestionnaireRecord(native.lastQuestionnaireRecord);
			lastResult = NativeArrayToList<s32, s32>(native.lastResult, PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM);
			lastMultiResult = NativeArrayToList<s32, s32>(native.lastMultiResult, PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM);
        }

		virtual System::String^ ToString() new
		{
			return "���T�̃A���P�[�g���e�F" + Environment::NewLine +
				currentQuestionnaireRecord->ToString() + Environment::NewLine + Environment::NewLine +
				"��T�̃A���P�[�g���e�F" + Environment::NewLine +
				lastQuestionnaireRecord->ToString() + Environment::NewLine + Environment::NewLine +
				"��T�̌���(�����̌���)�F" + Join(lastResult, ", ") + Environment::NewLine +
				"��T�̌���(��������)�F" + Join(lastMultiResult, ", ");

		}
	};



	/// �������
    [Serializable()]
	public ref struct REF_PPW_LobbyQuestionCondition
	{
		s32 questionNo;
		s32 multiLanguageSummarizeId;

		REF_PPW_LobbyQuestionCondition()
		{
		}

		REF_PPW_LobbyQuestionCondition(const PPW_LobbyQuestionCondition& native)
		{
			questionNo = native.questionNo;
			multiLanguageSummarizeId = native.multiLanguageSummarizeId;
		}

		PPW_LobbyQuestionCondition ToNative()
		{
			PPW_LobbyQuestionCondition native = { questionNo, multiLanguageSummarizeId };
			return native;
		}

		bool IsConsistent()
		{
			return (questionNo >= 0 && questionNo < 60) ||
				IsFreeQuestion(questionNo);
		}
	};

	/// �X�y�V�����E�B�[�N���R�[�h
    [Serializable()]
	public ref struct REF_PPW_LobbySpecialWeekRecord
	{
		static const int SPECIAL_RECORD_SIZE = sizeof(PPW_LobbySpecialWeekRecord);
		static const int LOBBY_FREE_QUESTION_START_NO = PPW_LOBBY_FREE_QUESTION_START_NO;
		static const int LOBBY_FREE_QUESTION_NUM = PPW_LOBBY_FREE_QUESTION_NUM;
	private:
		s32 questionSerialNo;
		List<REF_PPW_LobbyQuestionCondition^>^ questionConditions;
	public:
		property s32 QuestionSerialNo
		{
			s32 get()
			{
				return questionSerialNo;
			}
			void set(s32 value)
			{
				questionSerialNo = value;
			}
		}

		property s32 QuestionNoJp
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::JP]->questionNo;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::JP]->questionNo = value;
			}
		}

		property s32 QuestionNoUs
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::EN]->questionNo;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::EN]->questionNo = value;
			}
		}


		property s32 QuestionNoFr
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::FR]->questionNo;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::FR]->questionNo = value;
			}
		}
		

		property s32 QuestionNoIt
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::IT]->questionNo;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::IT]->questionNo = value;
			}
		}
		

		property s32 QuestionNoDe
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::IT]->questionNo;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::IT]->questionNo = value;
			}
		}
		

		property s32 QuestionNoSp
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::SP]->questionNo;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::SP]->questionNo = value;
			}
		}
		

		property s32 QuestionNoKr
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::KR]->questionNo;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::KR]->questionNo = value;
			}
		}

		property s32 MultiLanguageSummarizeIdJp
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::JP]->multiLanguageSummarizeId;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::JP]->multiLanguageSummarizeId = value;
			}
		}

		property s32 MultiLanguageSummarizeIdUs
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::EN]->multiLanguageSummarizeId;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::EN]->multiLanguageSummarizeId = value;
			}
		}

		property s32 MultiLanguageSummarizeIdFr
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::FR]->multiLanguageSummarizeId;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::FR]->multiLanguageSummarizeId = value;
			}
		}

		property s32 MultiLanguageSummarizeIdIt
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::IT]->multiLanguageSummarizeId;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::IT]->multiLanguageSummarizeId = value;
			}
		}
		

		property s32 MultiLanguageSummarizeIdDe
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::DE]->multiLanguageSummarizeId;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::DE]->multiLanguageSummarizeId = value;
			}
		}

		property s32 MultiLanguageSummarizeIdSp
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::SP]->multiLanguageSummarizeId;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::SP]->multiLanguageSummarizeId = value;
			}
		}
		
		property s32 MultiLanguageSummarizeIdKr
		{
			s32 get()
			{
				return questionConditions[(int)Util::Language::KR]->multiLanguageSummarizeId;
			}
			void set(s32 value)
			{
				questionConditions[(int)Util::Language::KR]->multiLanguageSummarizeId = value;
			}
		}

        REF_PPW_LobbySpecialWeekRecord()
		{
			questionConditions = gcnew List<REF_PPW_LobbyQuestionCondition^>();
			for(int i=0; i<LANGUAGE_MAX; i++)
			{
				questionConditions->Add(gcnew REF_PPW_LobbyQuestionCondition());
			}
		}

        REF_PPW_LobbySpecialWeekRecord(IntPtr ptr, int size)
        {
            PPW_LobbySpecialWeekRecord native;
            if(size != sizeof(native))
            {
                throw gcnew System::Exception("�T�C�Y����v���܂���");
            }
            memcpy(&native, (void*)ptr, sizeof(native));

			questionSerialNo = native.questionSerialNo;
			questionConditions = NativeArrayToManageList<REF_PPW_LobbyQuestionCondition, PPW_LobbyQuestionCondition>(native.questionConditions, LANGUAGE_MAX);
        }
		
        array<Byte>^ ToByteArray()
        {
			PPW_LobbySpecialWeekRecord native = {
				questionSerialNo,
				{0}	// questionConditions ��ő��
			};
            memcpy(&native.questionConditions,
				DWCi_GetVectorBuffer(ListToNativeArray<PPW_LobbyQuestionCondition, REF_PPW_LobbyQuestionCondition>(questionConditions)),
                sizeof(native.questionConditions[0]) * LANGUAGE_MAX);


            return NativeTypeToCliByteArray(native);
        }

		bool IsConsistent()
		{
			for(int i = 0; i<LANGUAGE_MAX; i++)
			{
				if(!questionConditions[i]->IsConsistent())
				{
					return false;
				}
			}
			return true;
		}
	};

	/// �C�ӎ���
	public ref struct REF_PPW_LobbyFreeQuestion
	{
		static const int LOBBY_MAX_QUESTION_SENTENCE_LENGTH = PPW_LOBBY_MAX_QUESTION_SENTENCE_LENGTH;
		static const int LOBBY_QUESTIONNAIRE_ANSWER_NUM = PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM;
		static const int LOBBY_MAX_ANSWER_LENGTH = PPW_LOBBY_MAX_ANSWER_LENGTH;
		static const int NATIVE_SIZE = sizeof(PPW_LobbyFreeQuestion);

	private:
		s32 language;                   ///< �o�^���錾��
		s32 questionNo;                 ///< ����ԍ�
		String^ questionSentence;	///< �C�ӎ���B
		List<String^>^ answer;		///< �C�ӎ���̉񓚁B
	public:
		property s32 Language
		{
			s32 get()
			{
				return language;
			}
			void set(s32 value)
			{
				language = value;
			}
		}

		property s32 QuestionNo
		{
			s32 get()
			{
				return questionNo;
			}
			void set(s32 value)
			{
				questionNo = value;
			}
		}

		property String^ QuestionSentence
		{
			String^ get()
			{
				return questionSentence;
			}
			void set(String^ value)
			{
				questionSentence = value;
			}
		}
		property String^ Answer1
		{
			String^ get()
			{
				return answer[0];
			}
			void set(String^ value)
			{
				answer[0] = value;
			}
		}
		property String^ Answer2
		{
			String^ get()
			{
				return answer[1];
			}
			void set(String^ value)
			{
				answer[1] = value;
			}
		}
		property String^ Answer3
		{
			String^ get()
			{
				return answer[2];
			}
			void set(String^ value)
			{
				answer[2] = value;
			}
		}


		REF_PPW_LobbyFreeQuestion(s32 questionNo_, s32 language_, String^ questionSentence_, List<String^>^ answer_)
		{
			language = language_;
			questionNo = questionNo_;
			questionSentence = questionSentence_;
			answer = answer_;
		}

		// XML�V���A���C�Y�ŕK�v
		REF_PPW_LobbyFreeQuestion()
		{
			answer = gcnew List<String^>();
			for(int i=0; i<PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM; i++)
			{
				answer->Add("");
			}
		}
		
        REF_PPW_LobbyFreeQuestion(IntPtr ptr, int size)
        {
            PPW_LobbyFreeQuestion native;
            if(size != sizeof(native))
            {
                throw gcnew Exception("�T�C�Y����v���܂���");
            }
            memcpy(&native, (void*)ptr, sizeof(native));

			questionNo = native.questionNo;
			language = native.language;

			// ���╶�̃}�[�V�������O
			{
				List<u16>^ questionSentenceList = NativeArrayToList<u16, u16>(native.questionSentence, PPW_LOBBY_MAX_QUESTION_SENTENCE_LENGTH);
				List<u8>^ bytes = Util::UShortsToBytes(questionSentenceList, false);
				String^ str = Encoding::Unicode->GetString(bytes->ToArray());

				try
				{
					questionSentence = Util::ConvLFToMacro(Util::TrimNullString(
						DpwUtil::Tools->PokecodeToUnicodeWithoutCheckLF(Encoding::Unicode->GetString(bytes->ToArray()))));
				}
				catch(Exception^)
				{
					throw gcnew Exception("�T�[�o�ɐݒ肪�Z�b�g����Ă��Ȃ����A���Ă��܂��B�����l���Z�b�g���Ă��������B");
				}
			}

			// �񓚂̃}�[�V�������O
			answer = gcnew List<String^>();
			for(int i=0; i<PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM; i++)
			{
				List<u16>^ answerList = NativeArrayToList<u16, u16>(native.answer[i], PPW_LOBBY_MAX_ANSWER_LENGTH);
				List<u8>^ bytes = Util::UShortsToBytes(answerList, false);
				answer->Add(Util::TrimNullString(DpwUtil::Tools->PokecodeToUnicode(Encoding::Unicode->GetString(bytes->ToArray()))));
			}
        }

        array<Byte>^ ToByteArray()
        {
			PPW_LobbyFreeQuestion native = {
				language,
				questionNo,
				{0},	// questionSentence ��ő��
				{0}		// answer ��ő��
			};

			// String����List<u16>�֕ϊ�
            String^ cur = "";
            String^ questionSentencePoke = "";
			List<String^>^ answerPokes = gcnew List<String^>();
            try
            {
				cur = questionSentence;
				cur = Util::ConvMacroToLF(cur);	// <LF>������s�R�[�h�ɕϊ�����
				questionSentencePoke = DpwUtil::Tools->UnicodeToPokecode(cur);
				for(int i=0; i<PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM; i++)
				{
					cur = answer[i];
					answerPokes->Add(DpwUtil::Tools->UnicodeToPokecode(cur));
				}
            }
            catch (Exception^ e)
            {
				MessageBox::Show("�u" + cur + "�v�̓Q�[���������R�[�h�֕ϊ��ł��܂���B");
                throw e;
            }
			List<u16>^ questionSentenceList = Util::UnicodeStringToUShortList(questionSentencePoke);
			List<List<u16>^>^ answerList = gcnew List<List<u16>^>();
			for(int i=0; i<PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM; i++)
			{
				answerList->Add(Util::UnicodeStringToUShortList(answerPokes[i]));
			}
			
			// List<u16>����l�C�e�B�u�z���
            memcpy(&native.questionSentence,
				DWCi_GetVectorBuffer(ListToNativeArray(questionSentenceList)),
                sizeof(native.questionSentence[0]) * PPW_LOBBY_MAX_QUESTION_SENTENCE_LENGTH);

			for(int i=0; i<PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM; i++)
			{
				memcpy(&native.answer[i],
					DWCi_GetVectorBuffer(ListToNativeArray(answerList[i])),
					sizeof(native.answer[0]));
			}
			
            return NativeTypeToCliByteArray(native);
        }

		bool IsConsistent()
		{
			return questionNo >= PPW_LOBBY_FREE_QUESTION_START_NO &&
				questionNo < PPW_LOBBY_FREE_QUESTION_START_NO + PPW_LOBBY_FREE_QUESTION_NUM &&
				this->questionSentence->Length <= 108 &&
				answer[0]->Length <= 16 &&
				answer[1]->Length <= 16 &&
				answer[2]->Length <= 16;
		}
	};
}