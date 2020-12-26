#pragma once
#include <Windows.h>
#include <fstream>

namespace ODEV2 {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
		//VALUES

		int imageWidth, imageHeight;
		long imageSize, newSize;
		LPCTSTR input, output;
		BYTE* image;
		BYTE* processedImage;

		bool intensityControl = FALSE;
		bool loadControl = FALSE;
		bool histogramControl = FALSE;
		bool histogram3dControl = FALSE;
		bool kmeansControl = FALSE;
		bool markerControl = FALSE;

		int processCount = 1;
		int* histogram = new int[256];
		int* histogram3d = new int[256 * 256 * 256];
		int* markedImage;
		int objectCount;
		unsigned char* image255;








	private: System::Windows::Forms::PictureBox^ pictureBox2;
	private: System::Windows::Forms::Button^ intensityButton;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart1;

	private: System::Windows::Forms::Button^ histogramButton;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ kmeansButton;
	private: System::Windows::Forms::Button^ restartButton;
	private: System::Windows::Forms::ComboBox^ clusterCountMenu;
	private: System::Windows::Forms::Button^ histogram3dButton;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart2;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Button^ markerButton;
	private: System::Windows::Forms::Button^ erosionButton;
	private: System::Windows::Forms::Button^ dilationButton;
	private: System::Windows::Forms::Button^ meanButton;
	private: System::Windows::Forms::Button^ medianButton;
	private: System::Windows::Forms::Button^ learningButton;
	private: System::Windows::Forms::Button^ findingButton;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Button^ cannyButton;






	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;

	public:
		MyForm(void)
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
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Button^ loadButton;
	private: System::Windows::Forms::Button^ saveButton;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->loadButton = (gcnew System::Windows::Forms::Button());
			this->saveButton = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->intensityButton = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->histogramButton = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->kmeansButton = (gcnew System::Windows::Forms::Button());
			this->restartButton = (gcnew System::Windows::Forms::Button());
			this->clusterCountMenu = (gcnew System::Windows::Forms::ComboBox());
			this->histogram3dButton = (gcnew System::Windows::Forms::Button());
			this->chart2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->markerButton = (gcnew System::Windows::Forms::Button());
			this->erosionButton = (gcnew System::Windows::Forms::Button());
			this->dilationButton = (gcnew System::Windows::Forms::Button());
			this->meanButton = (gcnew System::Windows::Forms::Button());
			this->medianButton = (gcnew System::Windows::Forms::Button());
			this->learningButton = (gcnew System::Windows::Forms::Button());
			this->findingButton = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->cannyButton = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::SystemColors::ControlDark;
			this->pictureBox1->Location = System::Drawing::Point(12, 51);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(600, 475);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// loadButton
			// 
			this->loadButton->Location = System::Drawing::Point(12, 637);
			this->loadButton->Name = L"loadButton";
			this->loadButton->Size = System::Drawing::Size(46, 23);
			this->loadButton->TabIndex = 1;
			this->loadButton->Text = L"LOAD";
			this->loadButton->UseVisualStyleBackColor = true;
			this->loadButton->Click += gcnew System::EventHandler(this, &MyForm::loadButton_Click);
			// 
			// saveButton
			// 
			this->saveButton->Location = System::Drawing::Point(1101, 637);
			this->saveButton->Name = L"saveButton";
			this->saveButton->Size = System::Drawing::Size(44, 23);
			this->saveButton->TabIndex = 2;
			this->saveButton->Text = L"SAVE";
			this->saveButton->UseVisualStyleBackColor = true;
			this->saveButton->Click += gcnew System::EventHandler(this, &MyForm::saveButton_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// pictureBox2
			// 
			this->pictureBox2->BackColor = System::Drawing::SystemColors::ControlDark;
			this->pictureBox2->Location = System::Drawing::Point(618, 51);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(600, 475);
			this->pictureBox2->TabIndex = 3;
			this->pictureBox2->TabStop = false;
			// 
			// intensityButton
			// 
			this->intensityButton->Location = System::Drawing::Point(64, 637);
			this->intensityButton->Name = L"intensityButton";
			this->intensityButton->Size = System::Drawing::Size(75, 23);
			this->intensityButton->TabIndex = 4;
			this->intensityButton->Text = L"INTENSITY";
			this->intensityButton->UseVisualStyleBackColor = true;
			this->intensityButton->Click += gcnew System::EventHandler(this, &MyForm::intensityButton_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(74, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Original Image";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(615, 9);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(89, 13);
			this->label2->TabIndex = 6;
			this->label2->Text = L"Processed Image";
			// 
			// chart1
			// 
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(1224, 51);
			this->chart1->Name = L"chart1";
			series1->ChartArea = L"ChartArea1";
			series1->Legend = L"Legend1";
			series1->Name = L"Pixels";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(363, 189);
			this->chart1->TabIndex = 7;
			this->chart1->Text = L"chart1";
			// 
			// histogramButton
			// 
			this->histogramButton->Location = System::Drawing::Point(145, 637);
			this->histogramButton->Name = L"histogramButton";
			this->histogramButton->Size = System::Drawing::Size(99, 23);
			this->histogramButton->TabIndex = 9;
			this->histogramButton->Text = L"HISTOGRAM";
			this->histogramButton->UseVisualStyleBackColor = true;
			this->histogramButton->Click += gcnew System::EventHandler(this, &MyForm::histogramButton_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(1224, 35);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(82, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Histogram Chart";
			// 
			// kmeansButton
			// 
			this->kmeansButton->Location = System::Drawing::Point(350, 637);
			this->kmeansButton->Name = L"kmeansButton";
			this->kmeansButton->Size = System::Drawing::Size(75, 23);
			this->kmeansButton->TabIndex = 11;
			this->kmeansButton->Text = L"K-MEANS";
			this->kmeansButton->UseVisualStyleBackColor = true;
			this->kmeansButton->Click += gcnew System::EventHandler(this, &MyForm::kmeansButton_Click);
			// 
			// restartButton
			// 
			this->restartButton->Location = System::Drawing::Point(1151, 637);
			this->restartButton->Name = L"restartButton";
			this->restartButton->Size = System::Drawing::Size(67, 23);
			this->restartButton->TabIndex = 12;
			this->restartButton->Text = L"RESTART";
			this->restartButton->UseVisualStyleBackColor = true;
			this->restartButton->Click += gcnew System::EventHandler(this, &MyForm::restartButton_Click);
			// 
			// clusterCountMenu
			// 
			this->clusterCountMenu->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->clusterCountMenu->FormattingEnabled = true;
			this->clusterCountMenu->Items->AddRange(gcnew cli::array< System::Object^  >(9) {
				L"2", L"3", L"4", L"5", L"6", L"7", L"8",
					L"9", L"10"
			});
			this->clusterCountMenu->Location = System::Drawing::Point(431, 639);
			this->clusterCountMenu->Name = L"clusterCountMenu";
			this->clusterCountMenu->Size = System::Drawing::Size(121, 21);
			this->clusterCountMenu->TabIndex = 13;
			// 
			// histogram3dButton
			// 
			this->histogram3dButton->Location = System::Drawing::Point(145, 669);
			this->histogram3dButton->Name = L"histogram3dButton";
			this->histogram3dButton->Size = System::Drawing::Size(99, 23);
			this->histogram3dButton->TabIndex = 14;
			this->histogram3dButton->Text = L"HISTOGRAM 3D";
			this->histogram3dButton->UseVisualStyleBackColor = true;
			this->histogram3dButton->Click += gcnew System::EventHandler(this, &MyForm::histogram3dButton_Click);
			// 
			// chart2
			// 
			chartArea2->Name = L"ChartArea1";
			this->chart2->ChartAreas->Add(chartArea2);
			legend2->Name = L"Legend1";
			this->chart2->Legends->Add(legend2);
			this->chart2->Location = System::Drawing::Point(1224, 259);
			this->chart2->Name = L"chart2";
			this->chart2->PaletteCustomColors = gcnew cli::array< System::Drawing::Color >(1) { System::Drawing::Color::Red };
			series2->ChartArea = L"ChartArea1";
			series2->Legend = L"Legend1";
			series2->Name = L"Pixels";
			this->chart2->Series->Add(series2);
			this->chart2->Size = System::Drawing::Size(363, 189);
			this->chart2->TabIndex = 15;
			this->chart2->Text = L"chart2";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(1224, 243);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(99, 13);
			this->label4->TabIndex = 16;
			this->label4->Text = L"Histogram 3D Chart";
			// 
			// markerButton
			// 
			this->markerButton->Location = System::Drawing::Point(639, 637);
			this->markerButton->Name = L"markerButton";
			this->markerButton->Size = System::Drawing::Size(75, 23);
			this->markerButton->TabIndex = 17;
			this->markerButton->Text = L"MARKER";
			this->markerButton->UseVisualStyleBackColor = true;
			this->markerButton->Click += gcnew System::EventHandler(this, &MyForm::markerButton_Click);
			// 
			// erosionButton
			// 
			this->erosionButton->Location = System::Drawing::Point(558, 637);
			this->erosionButton->Name = L"erosionButton";
			this->erosionButton->Size = System::Drawing::Size(75, 23);
			this->erosionButton->TabIndex = 18;
			this->erosionButton->Text = L"EROSION";
			this->erosionButton->UseVisualStyleBackColor = true;
			this->erosionButton->Click += gcnew System::EventHandler(this, &MyForm::erosionButton_Click);
			// 
			// dilationButton
			// 
			this->dilationButton->Location = System::Drawing::Point(558, 669);
			this->dilationButton->Name = L"dilationButton";
			this->dilationButton->Size = System::Drawing::Size(75, 23);
			this->dilationButton->TabIndex = 19;
			this->dilationButton->Text = L"DILATION";
			this->dilationButton->UseVisualStyleBackColor = true;
			this->dilationButton->Click += gcnew System::EventHandler(this, &MyForm::dilationButton_Click);
			// 
			// meanButton
			// 
			this->meanButton->Location = System::Drawing::Point(250, 637);
			this->meanButton->Name = L"meanButton";
			this->meanButton->Size = System::Drawing::Size(94, 23);
			this->meanButton->TabIndex = 20;
			this->meanButton->Text = L"MEAN";
			this->meanButton->UseVisualStyleBackColor = true;
			this->meanButton->Click += gcnew System::EventHandler(this, &MyForm::meanButton_Click);
			// 
			// medianButton
			// 
			this->medianButton->Location = System::Drawing::Point(250, 669);
			this->medianButton->Name = L"medianButton";
			this->medianButton->Size = System::Drawing::Size(94, 23);
			this->medianButton->TabIndex = 21;
			this->medianButton->Text = L"MEDIAN";
			this->medianButton->UseVisualStyleBackColor = true;
			this->medianButton->Click += gcnew System::EventHandler(this, &MyForm::medianButton_Click);
			// 
			// learningButton
			// 
			this->learningButton->Location = System::Drawing::Point(720, 637);
			this->learningButton->Name = L"learningButton";
			this->learningButton->Size = System::Drawing::Size(75, 23);
			this->learningButton->TabIndex = 22;
			this->learningButton->Text = L"LEARNING";
			this->learningButton->UseVisualStyleBackColor = true;
			this->learningButton->Click += gcnew System::EventHandler(this, &MyForm::learningButton_Click);
			// 
			// findingButton
			// 
			this->findingButton->Location = System::Drawing::Point(801, 637);
			this->findingButton->Name = L"findingButton";
			this->findingButton->Size = System::Drawing::Size(75, 23);
			this->findingButton->TabIndex = 23;
			this->findingButton->Text = L"FINDING";
			this->findingButton->UseVisualStyleBackColor = true;
			this->findingButton->Click += gcnew System::EventHandler(this, &MyForm::findingButton_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->BackColor = System::Drawing::Color::Lime;
			this->label5->ForeColor = System::Drawing::SystemColors::ControlText;
			this->label5->Location = System::Drawing::Point(1224, 466);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(78, 13);
			this->label5->TabIndex = 24;
			this->label5->Text = L"Yeþil Mercimek";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::Red;
			this->label6->Location = System::Drawing::Point(1308, 466);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(85, 13);
			this->label6->TabIndex = 25;
			this->label6->Text = L"Kýrmýzý Mercimek";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->BackColor = System::Drawing::Color::Yellow;
			this->label7->Location = System::Drawing::Point(1399, 466);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(33, 13);
			this->label7->TabIndex = 26;
			this->label7->Text = L"Pirinç";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->BackColor = System::Drawing::Color::Purple;
			this->label8->Location = System::Drawing::Point(1228, 489);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(43, 13);
			this->label8->TabIndex = 27;
			this->label8->Text = L"Fasülye";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->BackColor = System::Drawing::Color::Pink;
			this->label9->Location = System::Drawing::Point(1277, 489);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(49, 13);
			this->label9->TabIndex = 28;
			this->label9->Text = L"Çekirdek";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->BackColor = System::Drawing::Color::Blue;
			this->label10->Location = System::Drawing::Point(1332, 489);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(33, 13);
			this->label10->TabIndex = 29;
			this->label10->Text = L"Ceviz";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->label11->Location = System::Drawing::Point(1371, 489);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(33, 13);
			this->label11->TabIndex = 30;
			this->label11->Text = L"Eriþte";
			// 
			// cannyButton
			// 
			this->cannyButton->Location = System::Drawing::Point(64, 608);
			this->cannyButton->Name = L"cannyButton";
			this->cannyButton->Size = System::Drawing::Size(75, 23);
			this->cannyButton->TabIndex = 31;
			this->cannyButton->Text = L"CANNY";
			this->cannyButton->UseVisualStyleBackColor = true;
			this->cannyButton->Click += gcnew System::EventHandler(this, &MyForm::cannyButton_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(1599, 704);
			this->Controls->Add(this->cannyButton);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->findingButton);
			this->Controls->Add(this->learningButton);
			this->Controls->Add(this->medianButton);
			this->Controls->Add(this->meanButton);
			this->Controls->Add(this->dilationButton);
			this->Controls->Add(this->erosionButton);
			this->Controls->Add(this->markerButton);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->chart2);
			this->Controls->Add(this->histogram3dButton);
			this->Controls->Add(this->clusterCountMenu);
			this->Controls->Add(this->restartButton);
			this->Controls->Add(this->kmeansButton);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->histogramButton);
			this->Controls->Add(this->chart1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->intensityButton);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->saveButton);
			this->Controls->Add(this->loadButton);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void loadButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void saveButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void intensityButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void histogramButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void kmeansButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void restartButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void histogram3dButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void markerButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: Void Marker(int*, int, int);

	private: System::Void erosionButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void dilationButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void meanButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void medianButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void learningButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Void findingButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: double EuclideanDistance(double*, double*);

	private: System::Void cannyButton_Click(System::Object^ sender, System::EventArgs^ e);

	private: Void Save(BYTE* Buffer, int widthf, int heightf, long ssize);
	};
}
