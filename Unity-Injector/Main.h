#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "Utility.h"
#include "ProcessHelper.h"

#include <vector>
#include <string>

namespace UnityInjector {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Reflection;

	/// <summary>
	/// Summary for Main
	/// </summary>
	public ref class Main : public System::Windows::Forms::Form
	{
	public:
		Main(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Main()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^  profilesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveAsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  recentToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: System::Windows::Forms::GroupBox^  groupProcess;
	private: System::Windows::Forms::ComboBox^  comboProcesses;

	private: System::Windows::Forms::GroupBox^  groupDLLs;
	private: System::Windows::Forms::ListBox^  listDLLs;
	private: System::Windows::Forms::ContextMenuStrip^  ctxListDLLs;
	private: System::Windows::Forms::ToolStripMenuItem^  addToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  removeToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  dlgSelectDLLs;
	private: System::Windows::Forms::Button^  btnInject;

	private: System::Windows::Forms::ToolStripMenuItem^  patchMonoFunctionsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  entrypointDefinitionToolStripMenuItem;
	private: System::ComponentModel::IContainer^  components;





	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->profilesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->recentToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->patchMonoFunctionsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->groupProcess = (gcnew System::Windows::Forms::GroupBox());
			this->btnInject = (gcnew System::Windows::Forms::Button());
			this->comboProcesses = (gcnew System::Windows::Forms::ComboBox());
			this->groupDLLs = (gcnew System::Windows::Forms::GroupBox());
			this->listDLLs = (gcnew System::Windows::Forms::ListBox());
			this->ctxListDLLs = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->addToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->removeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dlgSelectDLLs = (gcnew System::Windows::Forms::OpenFileDialog());
			this->entrypointDefinitionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			this->groupProcess->SuspendLayout();
			this->groupDLLs->SuspendLayout();
			this->ctxListDLLs->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::SystemColors::Control;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->profilesToolStripMenuItem,
					this->toolsToolStripMenuItem, this->helpToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(284, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// profilesToolStripMenuItem
			// 
			this->profilesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->loadToolStripMenuItem,
					this->saveAsToolStripMenuItem, this->recentToolStripMenuItem
			});
			this->profilesToolStripMenuItem->Name = L"profilesToolStripMenuItem";
			this->profilesToolStripMenuItem->Size = System::Drawing::Size(58, 20);
			this->profilesToolStripMenuItem->Text = L"Profiles";
			// 
			// loadToolStripMenuItem
			// 
			this->loadToolStripMenuItem->Name = L"loadToolStripMenuItem";
			this->loadToolStripMenuItem->Size = System::Drawing::Size(114, 22);
			this->loadToolStripMenuItem->Text = L"Load";
			// 
			// saveAsToolStripMenuItem
			// 
			this->saveAsToolStripMenuItem->Name = L"saveAsToolStripMenuItem";
			this->saveAsToolStripMenuItem->Size = System::Drawing::Size(114, 22);
			this->saveAsToolStripMenuItem->Text = L"Save As";
			// 
			// recentToolStripMenuItem
			// 
			this->recentToolStripMenuItem->Name = L"recentToolStripMenuItem";
			this->recentToolStripMenuItem->Size = System::Drawing::Size(114, 22);
			this->recentToolStripMenuItem->Text = L"Recent";
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->patchMonoFunctionsToolStripMenuItem });
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->toolsToolStripMenuItem->Text = L"Tools";
			// 
			// patchMonoFunctionsToolStripMenuItem
			// 
			this->patchMonoFunctionsToolStripMenuItem->CheckOnClick = true;
			this->patchMonoFunctionsToolStripMenuItem->Name = L"patchMonoFunctionsToolStripMenuItem";
			this->patchMonoFunctionsToolStripMenuItem->Size = System::Drawing::Size(194, 22);
			this->patchMonoFunctionsToolStripMenuItem->Text = L"Patch Mono Functions";
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->entrypointDefinitionToolStripMenuItem,
					this->aboutToolStripMenuItem
			});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(183, 22);
			this->aboutToolStripMenuItem->Text = L"About";
			// 
			// groupProcess
			// 
			this->groupProcess->Controls->Add(this->btnInject);
			this->groupProcess->Controls->Add(this->comboProcesses);
			this->groupProcess->Location = System::Drawing::Point(12, 27);
			this->groupProcess->Name = L"groupProcess";
			this->groupProcess->Size = System::Drawing::Size(260, 67);
			this->groupProcess->TabIndex = 1;
			this->groupProcess->TabStop = false;
			this->groupProcess->Text = L"Process";
			// 
			// btnInject
			// 
			this->btnInject->Enabled = false;
			this->btnInject->Location = System::Drawing::Point(179, 19);
			this->btnInject->Name = L"btnInject";
			this->btnInject->Size = System::Drawing::Size(75, 23);
			this->btnInject->TabIndex = 1;
			this->btnInject->Text = L"Inject";
			this->btnInject->UseVisualStyleBackColor = true;
			this->btnInject->Click += gcnew System::EventHandler(this, &Main::btnInject_Click);
			// 
			// comboProcesses
			// 
			this->comboProcesses->FormattingEnabled = true;
			this->comboProcesses->Location = System::Drawing::Point(6, 19);
			this->comboProcesses->Name = L"comboProcesses";
			this->comboProcesses->Size = System::Drawing::Size(167, 21);
			this->comboProcesses->TabIndex = 0;
			this->comboProcesses->SelectedIndexChanged += gcnew System::EventHandler(this, &Main::comboProcesses_SelectedIndexChanged);
			this->comboProcesses->Enter += gcnew System::EventHandler(this, &Main::listProcesses_Enter);
			this->comboProcesses->Layout += gcnew System::Windows::Forms::LayoutEventHandler(this, &Main::listProcesses_Layout);
			// 
			// groupDLLs
			// 
			this->groupDLLs->Controls->Add(this->listDLLs);
			this->groupDLLs->Location = System::Drawing::Point(12, 100);
			this->groupDLLs->Name = L"groupDLLs";
			this->groupDLLs->Size = System::Drawing::Size(260, 150);
			this->groupDLLs->TabIndex = 2;
			this->groupDLLs->TabStop = false;
			this->groupDLLs->Text = L"DLLs";
			// 
			// listDLLs
			// 
			this->listDLLs->ContextMenuStrip = this->ctxListDLLs;
			this->listDLLs->FormattingEnabled = true;
			this->listDLLs->HorizontalScrollbar = true;
			this->listDLLs->Location = System::Drawing::Point(7, 22);
			this->listDLLs->Name = L"listDLLs";
			this->listDLLs->Size = System::Drawing::Size(247, 121);
			this->listDLLs->TabIndex = 0;
			// 
			// ctxListDLLs
			// 
			this->ctxListDLLs->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->addToolStripMenuItem,
					this->removeToolStripMenuItem
			});
			this->ctxListDLLs->Name = L"ctxListDLLs";
			this->ctxListDLLs->Size = System::Drawing::Size(118, 48);
			// 
			// addToolStripMenuItem
			// 
			this->addToolStripMenuItem->Name = L"addToolStripMenuItem";
			this->addToolStripMenuItem->Size = System::Drawing::Size(117, 22);
			this->addToolStripMenuItem->Text = L"Add";
			this->addToolStripMenuItem->Click += gcnew System::EventHandler(this, &Main::addToolStripMenuItem_Click);
			// 
			// removeToolStripMenuItem
			// 
			this->removeToolStripMenuItem->Name = L"removeToolStripMenuItem";
			this->removeToolStripMenuItem->Size = System::Drawing::Size(117, 22);
			this->removeToolStripMenuItem->Text = L"Remove";
			// 
			// dlgSelectDLLs
			// 
			this->dlgSelectDLLs->Filter = L"Dynamic Link Library | *.DLL";
			// 
			// entrypointDefinitionToolStripMenuItem
			// 
			this->entrypointDefinitionToolStripMenuItem->Name = L"entrypointDefinitionToolStripMenuItem";
			this->entrypointDefinitionToolStripMenuItem->Size = System::Drawing::Size(183, 22);
			this->entrypointDefinitionToolStripMenuItem->Text = L"Entrypoint definition";
			this->entrypointDefinitionToolStripMenuItem->Click += gcnew System::EventHandler(this, &Main::entrypointDefinitionToolStripMenuItem_Click);
			// 
			// Main
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 262);
			this->Controls->Add(this->groupDLLs);
			this->Controls->Add(this->groupProcess);
			this->Controls->Add(this->menuStrip1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->MainMenuStrip = this->menuStrip1;
			this->MaximizeBox = false;
			this->Name = L"Main";
			this->ShowIcon = false;
			this->Text = L"Unity Injector";
			this->Load += gcnew System::EventHandler(this, &Main::Main_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->groupProcess->ResumeLayout(false);
			this->groupDLLs->ResumeLayout(false);
			this->ctxListDLLs->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Main_Load(System::Object^  sender, System::EventArgs^  e)
	{
		// Set architecture
#ifdef _WIN64
		this->Text = "Unity Injector x64";
#else
		this->Text = "Unity Injector x86";
#endif
	}

	void UpdateListProcesses()
	{
		ProcessHelper::UpdateProcessInfo();
		auto processes = ProcessHelper::g_Processes;

		// Amazing race condition 10/10
		comboProcesses->Items->Clear();

		for (auto it = processes.begin(); it != processes.end(); it++) {
			auto pi = *it;

			// Can't inject DLLs into different architectures
			if (!pi->SameArchitecture())
				continue;

			// Mono required for injection
			if(pi->HasModule("mono.dll"))
				comboProcesses->Items->Add(gcnew String(pi->ProcessName.data()));
		}
	}

	System::String^ GetAssemblyName(System::String^ dll)
	{
		System::String^ assemblyName = "";

		try
		{
			assemblyName = AssemblyName::GetAssemblyName(dll)->Name;
			assemblyName = assemblyName->Replace("-", "_");
		}
		catch (...)
		{
			
		}

		return assemblyName;
	}

private: System::Void listProcesses_Enter(System::Object^  sender, System::EventArgs^  e) {
	UpdateListProcesses();
}
private: System::Void listProcesses_Layout(System::Object^  sender, System::Windows::Forms::LayoutEventArgs^  e) {
	//UpdateListProcesses();
}
private: System::Void addToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	System::Windows::Forms::DialogResult res = dlgSelectDLLs->ShowDialog();

	if (res != System::Windows::Forms::DialogResult::OK)
	{
		return;
	}

	for each (System::String^ dll in dlgSelectDLLs->FileNames)
	{
		if (GetAssemblyName(dll) != "")
			listDLLs->Items->Add(dll);
		else
			MessageBox::Show("Not a valid assembly!");
	}
}
private: System::Void btnInject_Click(System::Object^  sender, System::EventArgs^  e) {
	if (comboProcesses->SelectedIndex == -1)
		return;

	auto selProcess = comboProcesses->Items[comboProcesses->SelectedIndex];
	
	for (auto it = ProcessHelper::g_Processes.begin(); it != ProcessHelper::g_Processes.end(); it++) {
		ProcessHelper::ProcessInfo* pi = *it;

		if ((gcnew System::String(pi->ProcessName.data()))->Equals(selProcess))
		{
			for each (System::String^ dll in listDLLs->Items)
			{
				ProcessHelper::InjectDLL(Utility::ToString(dll), Utility::ToString(GetAssemblyName(dll)), pi);
			}
		}
	}

	UpdateListProcesses();
}
private: System::Void comboProcesses_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	btnInject->Enabled = comboProcesses->SelectedIndex != -1;
	UpdateListProcesses();
}
private: System::Void entrypointDefinitionToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	MessageBox::Show("Class Loader { static void Load(); }");
}
};
}
