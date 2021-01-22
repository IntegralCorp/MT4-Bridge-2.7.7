namespace TestCasesApp
{
  partial class ScenarioForm
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
      this.symbolText = new System.Windows.Forms.TextBox();
      this.label1 = new System.Windows.Forms.Label();
      this.opListBox = new System.Windows.Forms.CheckedListBox();
      this.tpUpDown = new System.Windows.Forms.NumericUpDown();
      this.label2 = new System.Windows.Forms.Label();
      this.label3 = new System.Windows.Forms.Label();
      this.slUpDown = new System.Windows.Forms.NumericUpDown();
      this.dataGridView1 = new System.Windows.Forms.DataGridView();
      this.Type = new System.Windows.Forms.DataGridViewTextBoxColumn();
      this.Lots = new System.Windows.Forms.DataGridViewTextBoxColumn();
      this.Price = new System.Windows.Forms.DataGridViewTextBoxColumn();
      this.Slippage = new System.Windows.Forms.DataGridViewTextBoxColumn();
      this.TP = new System.Windows.Forms.DataGridViewTextBoxColumn();
      this.SL = new System.Windows.Forms.DataGridViewTextBoxColumn();
      this.label4 = new System.Windows.Forms.Label();
      this.textScenario = new System.Windows.Forms.TextBox();
      this.label5 = new System.Windows.Forms.Label();
      this.lotsUpDown = new System.Windows.Forms.NumericUpDown();
      this.folderBox = new System.Windows.Forms.RichTextBox();
      this.btnCreate = new System.Windows.Forms.Button();
      this.label6 = new System.Windows.Forms.Label();
      this.slippageUpDown = new System.Windows.Forms.NumericUpDown();
      ((System.ComponentModel.ISupportInitialize)(this.tpUpDown)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.slUpDown)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.lotsUpDown)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.slippageUpDown)).BeginInit();
      this.SuspendLayout();
      // 
      // symbolText
      // 
      this.symbolText.Location = new System.Drawing.Point(132, 36);
      this.symbolText.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
      this.symbolText.Name = "symbolText";
      this.symbolText.Size = new System.Drawing.Size(195, 26);
      this.symbolText.TabIndex = 0;
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(62, 34);
      this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(65, 20);
      this.label1.TabIndex = 1;
      this.label1.Text = "Symbol:";
      // 
      // opListBox
      // 
      this.opListBox.FormattingEnabled = true;
      this.opListBox.Items.AddRange(new object[] {
            "OP_BUY",
            "OP_SELL",
            "OP_BUYLIMIT",
            "OP_SELLLIMIT",
            "OP_BUYSTOP",
            "OP_SELLSTOP"});
      this.opListBox.Location = new System.Drawing.Point(512, 63);
      this.opListBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
      this.opListBox.Name = "opListBox";
      this.opListBox.Size = new System.Drawing.Size(177, 142);
      this.opListBox.TabIndex = 3;
      this.opListBox.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.opListBox_ItemCheck);
      // 
      // tpUpDown
      // 
      this.tpUpDown.Location = new System.Drawing.Point(132, 81);
      this.tpUpDown.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
      this.tpUpDown.Name = "tpUpDown";
      this.tpUpDown.Size = new System.Drawing.Size(129, 26);
      this.tpUpDown.TabIndex = 4;
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(39, 82);
      this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(89, 20);
      this.label2.TabIndex = 5;
      this.label2.Text = "Take Profit:";
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(45, 110);
      this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(85, 20);
      this.label3.TabIndex = 7;
      this.label3.Text = "Stop Loss:";
      // 
      // slUpDown
      // 
      this.slUpDown.Location = new System.Drawing.Point(132, 109);
      this.slUpDown.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
      this.slUpDown.Name = "slUpDown";
      this.slUpDown.Size = new System.Drawing.Size(129, 26);
      this.slUpDown.TabIndex = 6;
      // 
      // dataGridView1
      // 
      dataGridViewCellStyle1.BackColor = System.Drawing.Color.Snow;
      this.dataGridView1.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
      this.dataGridView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.dataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.DisplayedCells;
      this.dataGridView1.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.DisplayedCells;
      this.dataGridView1.BackgroundColor = System.Drawing.SystemColors.ControlLightLight;
      this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Type,
            this.Lots,
            this.Price,
            this.Slippage,
            this.TP,
            this.SL});
      this.dataGridView1.Location = new System.Drawing.Point(7, 298);
      this.dataGridView1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
      this.dataGridView1.MultiSelect = false;
      this.dataGridView1.Name = "dataGridView1";
      this.dataGridView1.RowHeadersVisible = false;
      this.dataGridView1.RowHeadersWidth = 50;
      this.dataGridView1.RowTemplate.Height = 40;
      this.dataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
      this.dataGridView1.Size = new System.Drawing.Size(680, 250);
      this.dataGridView1.TabIndex = 8;
      // 
      // Type
      // 
      this.Type.DataPropertyName = "Type";
      this.Type.HeaderText = "Type";
      this.Type.MinimumWidth = 12;
      this.Type.Name = "Type";
      this.Type.Width = 79;
      // 
      // Lots
      // 
      this.Lots.DataPropertyName = "Lots";
      this.Lots.HeaderText = "Lots";
      this.Lots.MinimumWidth = 12;
      this.Lots.Name = "Lots";
      this.Lots.Width = 76;
      // 
      // Price
      // 
      this.Price.DataPropertyName = "Price";
      this.Price.HeaderText = "Price";
      this.Price.MinimumWidth = 12;
      this.Price.Name = "Price";
      this.Price.Width = 80;
      // 
      // Slippage
      // 
      this.Slippage.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
      this.Slippage.DataPropertyName = "Slippage";
      this.Slippage.HeaderText = "Slippage";
      this.Slippage.MinimumWidth = 12;
      this.Slippage.Name = "Slippage";
      this.Slippage.Width = 107;
      // 
      // TP
      // 
      this.TP.DataPropertyName = "TP";
      this.TP.HeaderText = "T/P";
      this.TP.MinimumWidth = 12;
      this.TP.Name = "TP";
      this.TP.Width = 68;
      // 
      // SL
      // 
      this.SL.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
      this.SL.DataPropertyName = "SL";
      this.SL.HeaderText = "S/L";
      this.SL.MinimumWidth = 12;
      this.SL.Name = "SL";
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Location = new System.Drawing.Point(414, 34);
      this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(76, 20);
      this.label4.TabIndex = 10;
      this.label4.Text = "Scenario:";
      // 
      // textScenario
      // 
      this.textScenario.Location = new System.Drawing.Point(494, 34);
      this.textScenario.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
      this.textScenario.Name = "textScenario";
      this.textScenario.Size = new System.Drawing.Size(195, 26);
      this.textScenario.TabIndex = 9;
      this.textScenario.Text = "TC1";
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Location = new System.Drawing.Point(86, 169);
      this.label5.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(44, 20);
      this.label5.TabIndex = 12;
      this.label5.Text = "Lots:";
      // 
      // lotsUpDown
      // 
      this.lotsUpDown.DecimalPlaces = 2;
      this.lotsUpDown.Location = new System.Drawing.Point(132, 168);
      this.lotsUpDown.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
      this.lotsUpDown.Name = "lotsUpDown";
      this.lotsUpDown.Size = new System.Drawing.Size(129, 26);
      this.lotsUpDown.TabIndex = 11;
      this.lotsUpDown.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
      // 
      // folderBox
      // 
      this.folderBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.folderBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::TestCasesApp.Properties.Settings.Default, "scenarioFolder", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
      this.folderBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.folderBox.Location = new System.Drawing.Point(7, 252);
      this.folderBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
      this.folderBox.Name = "folderBox";
      this.folderBox.Size = new System.Drawing.Size(403, 32);
      this.folderBox.TabIndex = 13;
      this.folderBox.Text = global::TestCasesApp.Properties.Settings.Default.scenarioFolder;
      this.folderBox.TextChanged += new System.EventHandler(this.folderBox_TextChanged);
      // 
      // btnCreate
      // 
      this.btnCreate.Location = new System.Drawing.Point(474, 235);
      this.btnCreate.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
      this.btnCreate.Name = "btnCreate";
      this.btnCreate.Size = new System.Drawing.Size(213, 59);
      this.btnCreate.TabIndex = 14;
      this.btnCreate.Text = "Append To File";
      this.btnCreate.UseVisualStyleBackColor = true;
      this.btnCreate.Click += new System.EventHandler(this.btnCreate_Click);
      // 
      // label6
      // 
      this.label6.AutoSize = true;
      this.label6.Location = new System.Drawing.Point(52, 200);
      this.label6.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
      this.label6.Name = "label6";
      this.label6.Size = new System.Drawing.Size(75, 20);
      this.label6.TabIndex = 16;
      this.label6.Text = "Slippage:";
      // 
      // slippageUpDown
      // 
      this.slippageUpDown.Location = new System.Drawing.Point(132, 199);
      this.slippageUpDown.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
      this.slippageUpDown.Name = "slippageUpDown";
      this.slippageUpDown.Size = new System.Drawing.Size(129, 26);
      this.slippageUpDown.TabIndex = 15;
      this.slippageUpDown.Value = new decimal(new int[] {
            15,
            0,
            0,
            0});
      // 
      // ScenarioForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(694, 556);
      this.Controls.Add(this.label6);
      this.Controls.Add(this.slippageUpDown);
      this.Controls.Add(this.btnCreate);
      this.Controls.Add(this.folderBox);
      this.Controls.Add(this.label5);
      this.Controls.Add(this.lotsUpDown);
      this.Controls.Add(this.label4);
      this.Controls.Add(this.textScenario);
      this.Controls.Add(this.dataGridView1);
      this.Controls.Add(this.label3);
      this.Controls.Add(this.slUpDown);
      this.Controls.Add(this.label2);
      this.Controls.Add(this.tpUpDown);
      this.Controls.Add(this.opListBox);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.symbolText);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
      this.MaximizeBox = false;
      this.Name = "ScenarioForm";
      this.Text = "Scenario Form";
      this.Load += new System.EventHandler(this.ScenarioForm_Load);
      ((System.ComponentModel.ISupportInitialize)(this.tpUpDown)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.slUpDown)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.lotsUpDown)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.slippageUpDown)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.TextBox symbolText;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.CheckedListBox opListBox;
    private System.Windows.Forms.NumericUpDown tpUpDown;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.NumericUpDown slUpDown;
    private System.Windows.Forms.DataGridView dataGridView1;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.TextBox textScenario;
    private System.Windows.Forms.Label label5;
    private System.Windows.Forms.NumericUpDown lotsUpDown;
    private System.Windows.Forms.RichTextBox folderBox;
    private System.Windows.Forms.Button btnCreate;
    private System.Windows.Forms.Label label6;
    private System.Windows.Forms.NumericUpDown slippageUpDown;
    private System.Windows.Forms.DataGridViewTextBoxColumn Type;
    private System.Windows.Forms.DataGridViewTextBoxColumn Lots;
    private System.Windows.Forms.DataGridViewTextBoxColumn Price;
    private System.Windows.Forms.DataGridViewTextBoxColumn Slippage;
    private System.Windows.Forms.DataGridViewTextBoxColumn TP;
    private System.Windows.Forms.DataGridViewTextBoxColumn SL;
  }
}