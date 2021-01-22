namespace TestCasesApp
{
  partial class Form1
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
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle7 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle8 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle10 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle11 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle9 = new System.Windows.Forms.DataGridViewCellStyle();
			this.btnOrders = new System.Windows.Forms.Button();
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
			this.messagesGridView = new System.Windows.Forms.DataGridView();
			this.Time = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.InOut = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Message = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.folderBox = new System.Windows.Forms.RichTextBox();
			this.panel1 = new System.Windows.Forms.Panel();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.panel4 = new System.Windows.Forms.Panel();
			this.dictGridView = new System.Windows.Forms.DataGridView();
			this.FieldTag = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.FieldName = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.FieldValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.FieldExpected = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.panel3 = new System.Windows.Forms.Panel();
			this.panel2 = new System.Windows.Forms.Panel();
			this.btnInstruments = new System.Windows.Forms.Button();
			this.ordersGridView = new System.Windows.Forms.DataGridView();
			this.Scenario = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Order = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Symbol = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MainOrder = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Amount = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Cmd = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Action = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.OpenPrice = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ClosePrice = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Comment = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.statusStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.messagesGridView)).BeginInit();
			this.panel1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.panel4.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dictGridView)).BeginInit();
			this.panel2.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.ordersGridView)).BeginInit();
			this.SuspendLayout();
			// 
			// btnOrders
			// 
			this.btnOrders.AutoSize = true;
			this.btnOrders.Location = new System.Drawing.Point(12, 12);
			this.btnOrders.Name = "btnOrders";
			this.btnOrders.Size = new System.Drawing.Size(246, 52);
			this.btnOrders.TabIndex = 0;
			this.btnOrders.Text = "Read Files";
			this.btnOrders.UseVisualStyleBackColor = true;
			this.btnOrders.Click += new System.EventHandler(this.btnOrders_Click);
			// 
			// statusStrip1
			// 
			this.statusStrip1.ImageScalingSize = new System.Drawing.Size(40, 40);
			this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
			this.statusStrip1.Location = new System.Drawing.Point(0, 1014);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(2068, 54);
			this.statusStrip1.TabIndex = 1;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
			this.toolStripStatusLabel1.Size = new System.Drawing.Size(119, 41);
			this.toolStripStatusLabel1.Text = "Waiting";
			// 
			// messagesGridView
			// 
			this.messagesGridView.AllowUserToAddRows = false;
			this.messagesGridView.AllowUserToDeleteRows = false;
			dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.ControlLight;
			this.messagesGridView.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
			this.messagesGridView.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
			this.messagesGridView.BackgroundColor = System.Drawing.SystemColors.ControlLightLight;
			dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle2.BackColor = System.Drawing.SystemColors.Control;
			dataGridViewCellStyle2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle2.ForeColor = System.Drawing.SystemColors.WindowText;
			dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.Highlight;
			dataGridViewCellStyle2.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
			dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.messagesGridView.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
			this.messagesGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.messagesGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Time,
            this.InOut,
            this.Message});
			dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle3.BackColor = System.Drawing.SystemColors.Window;
			dataGridViewCellStyle3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle3.ForeColor = System.Drawing.SystemColors.ControlText;
			dataGridViewCellStyle3.SelectionBackColor = System.Drawing.SystemColors.Highlight;
			dataGridViewCellStyle3.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
			dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
			this.messagesGridView.DefaultCellStyle = dataGridViewCellStyle3;
			this.messagesGridView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.messagesGridView.Location = new System.Drawing.Point(0, 0);
			this.messagesGridView.MultiSelect = false;
			this.messagesGridView.Name = "messagesGridView";
			this.messagesGridView.ReadOnly = true;
			dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle4.BackColor = System.Drawing.SystemColors.Control;
			dataGridViewCellStyle4.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle4.ForeColor = System.Drawing.SystemColors.WindowText;
			dataGridViewCellStyle4.SelectionBackColor = System.Drawing.SystemColors.Highlight;
			dataGridViewCellStyle4.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
			dataGridViewCellStyle4.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.messagesGridView.RowHeadersDefaultCellStyle = dataGridViewCellStyle4;
			this.messagesGridView.RowHeadersVisible = false;
			this.messagesGridView.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.AutoSizeToDisplayedHeaders;
			this.messagesGridView.RowTemplate.DefaultCellStyle.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.messagesGridView.RowTemplate.Height = 40;
			this.messagesGridView.RowTemplate.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.messagesGridView.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this.messagesGridView.Size = new System.Drawing.Size(2068, 330);
			this.messagesGridView.TabIndex = 3;
			this.messagesGridView.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.messagesGridView_CellClick);
			this.messagesGridView.CellDoubleClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.messagesGridView_CellDoubleClick);
			this.messagesGridView.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler(this.messagesGridView_CellFormatting);
			// 
			// Time
			// 
			this.Time.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Time.DataPropertyName = "Time";
			this.Time.HeaderText = "Time";
			this.Time.MinimumWidth = 12;
			this.Time.Name = "Time";
			this.Time.ReadOnly = true;
			this.Time.Width = 200;
			// 
			// InOut
			// 
			this.InOut.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.InOut.DataPropertyName = "InOut";
			this.InOut.HeaderText = "To/From";
			this.InOut.MinimumWidth = 12;
			this.InOut.Name = "InOut";
			this.InOut.ReadOnly = true;
			this.InOut.Width = 75;
			// 
			// Message
			// 
			this.Message.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
			this.Message.DataPropertyName = "Message";
			this.Message.HeaderText = "Message";
			this.Message.MinimumWidth = 12;
			this.Message.Name = "Message";
			this.Message.ReadOnly = true;
			// 
			// folderBox
			// 
			this.folderBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.folderBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::TestCasesApp.Properties.Settings.Default, "folder", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.folderBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.folderBox.Location = new System.Drawing.Point(12, 70);
			this.folderBox.Name = "folderBox";
			this.folderBox.Size = new System.Drawing.Size(714, 48);
			this.folderBox.TabIndex = 5;
			this.folderBox.Text = global::TestCasesApp.Properties.Settings.Default.folder;
			this.folderBox.TextChanged += new System.EventHandler(this.folderBox_TextChanged);
			// 
			// panel1
			// 
			this.panel1.AutoScroll = true;
			this.panel1.Controls.Add(this.messagesGridView);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.panel1.Location = new System.Drawing.Point(0, 684);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(2068, 330);
			this.panel1.TabIndex = 8;
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(0, 0);
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.panel4);
			this.splitContainer1.Panel1.Controls.Add(this.panel3);
			this.splitContainer1.Panel1.Controls.Add(this.panel2);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.ordersGridView);
			this.splitContainer1.Size = new System.Drawing.Size(2068, 684);
			this.splitContainer1.SplitterDistance = 761;
			this.splitContainer1.SplitterIncrement = 8;
			this.splitContainer1.TabIndex = 9;
			// 
			// panel4
			// 
			this.panel4.Controls.Add(this.dictGridView);
			this.panel4.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panel4.Location = new System.Drawing.Point(0, 141);
			this.panel4.Name = "panel4";
			this.panel4.Size = new System.Drawing.Size(761, 508);
			this.panel4.TabIndex = 2;
			// 
			// dictGridView
			// 
			this.dictGridView.AllowUserToAddRows = false;
			this.dictGridView.AllowUserToDeleteRows = false;
			this.dictGridView.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.DisplayedCells;
			this.dictGridView.BackgroundColor = System.Drawing.SystemColors.ButtonHighlight;
			dataGridViewCellStyle5.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle5.BackColor = System.Drawing.SystemColors.Control;
			dataGridViewCellStyle5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.1F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle5.ForeColor = System.Drawing.SystemColors.WindowText;
			dataGridViewCellStyle5.SelectionBackColor = System.Drawing.SystemColors.Highlight;
			dataGridViewCellStyle5.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
			dataGridViewCellStyle5.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.dictGridView.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle5;
			this.dictGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dictGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.FieldTag,
            this.FieldName,
            this.FieldValue,
            this.FieldExpected});
			dataGridViewCellStyle6.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle6.BackColor = System.Drawing.SystemColors.Window;
			dataGridViewCellStyle6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.1F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle6.ForeColor = System.Drawing.SystemColors.ControlText;
			dataGridViewCellStyle6.SelectionBackColor = System.Drawing.SystemColors.Highlight;
			dataGridViewCellStyle6.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
			dataGridViewCellStyle6.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
			this.dictGridView.DefaultCellStyle = dataGridViewCellStyle6;
			this.dictGridView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dictGridView.GridColor = System.Drawing.SystemColors.ControlLightLight;
			this.dictGridView.Location = new System.Drawing.Point(0, 0);
			this.dictGridView.Name = "dictGridView";
			this.dictGridView.ReadOnly = true;
			dataGridViewCellStyle7.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle7.BackColor = System.Drawing.SystemColors.Control;
			dataGridViewCellStyle7.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.1F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle7.ForeColor = System.Drawing.SystemColors.WindowText;
			dataGridViewCellStyle7.SelectionBackColor = System.Drawing.SystemColors.Highlight;
			dataGridViewCellStyle7.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
			dataGridViewCellStyle7.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.dictGridView.RowHeadersDefaultCellStyle = dataGridViewCellStyle7;
			this.dictGridView.RowHeadersVisible = false;
			this.dictGridView.RowHeadersWidth = 102;
			this.dictGridView.RowTemplate.Height = 40;
			this.dictGridView.Size = new System.Drawing.Size(761, 508);
			this.dictGridView.TabIndex = 7;
			// 
			// FieldTag
			// 
			this.FieldTag.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.DisplayedCells;
			this.FieldTag.DataPropertyName = "FieldTag";
			this.FieldTag.HeaderText = "Tag";
			this.FieldTag.MinimumWidth = 75;
			this.FieldTag.Name = "FieldTag";
			this.FieldTag.ReadOnly = true;
			this.FieldTag.Width = 118;
			// 
			// FieldName
			// 
			this.FieldName.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.DisplayedCells;
			this.FieldName.DataPropertyName = "FieldName";
			this.FieldName.HeaderText = "Name";
			this.FieldName.MinimumWidth = 100;
			this.FieldName.Name = "FieldName";
			this.FieldName.ReadOnly = true;
			this.FieldName.Width = 144;
			// 
			// FieldValue
			// 
			this.FieldValue.DataPropertyName = "FieldValue";
			this.FieldValue.HeaderText = "Value";
			this.FieldValue.MinimumWidth = 100;
			this.FieldValue.Name = "FieldValue";
			this.FieldValue.ReadOnly = true;
			this.FieldValue.Width = 125;
			// 
			// FieldExpected
			// 
			this.FieldExpected.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
			this.FieldExpected.DataPropertyName = "FieldExpected";
			this.FieldExpected.HeaderText = "Expected";
			this.FieldExpected.MinimumWidth = 100;
			this.FieldExpected.Name = "FieldExpected";
			this.FieldExpected.ReadOnly = true;
			// 
			// panel3
			// 
			this.panel3.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.panel3.Location = new System.Drawing.Point(0, 649);
			this.panel3.Name = "panel3";
			this.panel3.Size = new System.Drawing.Size(761, 35);
			this.panel3.TabIndex = 1;
			// 
			// panel2
			// 
			this.panel2.Controls.Add(this.btnInstruments);
			this.panel2.Controls.Add(this.folderBox);
			this.panel2.Controls.Add(this.btnOrders);
			this.panel2.Dock = System.Windows.Forms.DockStyle.Top;
			this.panel2.Location = new System.Drawing.Point(0, 0);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(761, 141);
			this.panel2.TabIndex = 0;
			// 
			// btnInstruments
			// 
			this.btnInstruments.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnInstruments.AutoSize = true;
			this.btnInstruments.Location = new System.Drawing.Point(478, 12);
			this.btnInstruments.Name = "btnInstruments";
			this.btnInstruments.Size = new System.Drawing.Size(264, 52);
			this.btnInstruments.TabIndex = 6;
			this.btnInstruments.Text = "Generate Scenario";
			this.btnInstruments.UseVisualStyleBackColor = true;
			this.btnInstruments.Click += new System.EventHandler(this.btnInstruments_Click);
			// 
			// ordersGridView
			// 
			this.ordersGridView.AllowUserToAddRows = false;
			this.ordersGridView.AllowUserToDeleteRows = false;
			this.ordersGridView.AllowUserToOrderColumns = true;
			this.ordersGridView.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
			this.ordersGridView.BackgroundColor = System.Drawing.SystemColors.ButtonHighlight;
			dataGridViewCellStyle8.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle8.BackColor = System.Drawing.SystemColors.Control;
			dataGridViewCellStyle8.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle8.ForeColor = System.Drawing.SystemColors.WindowText;
			dataGridViewCellStyle8.SelectionBackColor = System.Drawing.SystemColors.Highlight;
			dataGridViewCellStyle8.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
			dataGridViewCellStyle8.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.ordersGridView.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle8;
			this.ordersGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.ordersGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Scenario,
            this.Order,
            this.Symbol,
            this.MainOrder,
            this.Amount,
            this.Cmd,
            this.Action,
            this.OpenPrice,
            this.ClosePrice,
            this.Comment});
			dataGridViewCellStyle10.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle10.BackColor = System.Drawing.SystemColors.Window;
			dataGridViewCellStyle10.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle10.ForeColor = System.Drawing.SystemColors.ControlText;
			dataGridViewCellStyle10.SelectionBackColor = System.Drawing.SystemColors.Highlight;
			dataGridViewCellStyle10.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
			dataGridViewCellStyle10.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
			this.ordersGridView.DefaultCellStyle = dataGridViewCellStyle10;
			this.ordersGridView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ordersGridView.Location = new System.Drawing.Point(0, 0);
			this.ordersGridView.MultiSelect = false;
			this.ordersGridView.Name = "ordersGridView";
			this.ordersGridView.ReadOnly = true;
			this.ordersGridView.RowHeadersVisible = false;
			this.ordersGridView.RowHeadersWidth = 102;
			dataGridViewCellStyle11.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.ordersGridView.RowsDefaultCellStyle = dataGridViewCellStyle11;
			this.ordersGridView.RowTemplate.Height = 40;
			this.ordersGridView.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this.ordersGridView.Size = new System.Drawing.Size(1303, 684);
			this.ordersGridView.TabIndex = 4;
			this.ordersGridView.SelectionChanged += new System.EventHandler(this.ordersGridView_SelectionChanged);
			// 
			// Scenario
			// 
			this.Scenario.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
			this.Scenario.DataPropertyName = "Scenario";
			this.Scenario.HeaderText = "Scenario";
			this.Scenario.MinimumWidth = 50;
			this.Scenario.Name = "Scenario";
			this.Scenario.ReadOnly = true;
			this.Scenario.Width = 187;
			// 
			// Order
			// 
			this.Order.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
			this.Order.DataPropertyName = "Order";
			this.Order.HeaderText = "Order";
			this.Order.MinimumWidth = 50;
			this.Order.Name = "Order";
			this.Order.ReadOnly = true;
			this.Order.Width = 144;
			// 
			// Symbol
			// 
			this.Symbol.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
			this.Symbol.DataPropertyName = "Symbol";
			this.Symbol.HeaderText = "Symbol";
			this.Symbol.MinimumWidth = 50;
			this.Symbol.Name = "Symbol";
			this.Symbol.ReadOnly = true;
			this.Symbol.Width = 168;
			// 
			// MainOrder
			// 
			this.MainOrder.DataPropertyName = "MainOrder";
			this.MainOrder.HeaderText = "MainOrder";
			this.MainOrder.MinimumWidth = 12;
			this.MainOrder.Name = "MainOrder";
			this.MainOrder.ReadOnly = true;
			this.MainOrder.Width = 140;
			// 
			// Amount
			// 
			this.Amount.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
			this.Amount.DataPropertyName = "Amount";
			dataGridViewCellStyle9.Format = "N2";
			dataGridViewCellStyle9.NullValue = null;
			this.Amount.DefaultCellStyle = dataGridViewCellStyle9;
			this.Amount.HeaderText = "Amount";
			this.Amount.MinimumWidth = 100;
			this.Amount.Name = "Amount";
			this.Amount.ReadOnly = true;
			this.Amount.Width = 172;
			// 
			// Cmd
			// 
			this.Cmd.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
			this.Cmd.DataPropertyName = "Cmd";
			this.Cmd.HeaderText = "Cmd";
			this.Cmd.MinimumWidth = 100;
			this.Cmd.Name = "Cmd";
			this.Cmd.ReadOnly = true;
			this.Cmd.Width = 130;
			// 
			// Action
			// 
			this.Action.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.DisplayedCells;
			this.Action.DataPropertyName = "Action";
			this.Action.HeaderText = "Action";
			this.Action.MinimumWidth = 12;
			this.Action.Name = "Action";
			this.Action.ReadOnly = true;
			this.Action.Width = 154;
			// 
			// OpenPrice
			// 
			this.OpenPrice.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
			this.OpenPrice.DataPropertyName = "OpenPrice";
			this.OpenPrice.HeaderText = "OpenPrice";
			this.OpenPrice.MinimumWidth = 50;
			this.OpenPrice.Name = "OpenPrice";
			this.OpenPrice.ReadOnly = true;
			this.OpenPrice.Width = 209;
			// 
			// ClosePrice
			// 
			this.ClosePrice.DataPropertyName = "ClosePrice";
			this.ClosePrice.HeaderText = "ClosePrice";
			this.ClosePrice.MinimumWidth = 12;
			this.ClosePrice.Name = "ClosePrice";
			this.ClosePrice.ReadOnly = true;
			this.ClosePrice.Width = 125;
			// 
			// Comment
			// 
			this.Comment.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
			this.Comment.DataPropertyName = "Comment";
			this.Comment.HeaderText = "Comment";
			this.Comment.MinimumWidth = 100;
			this.Comment.Name = "Comment";
			this.Comment.ReadOnly = true;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(16F, 31F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSize = true;
			this.ClientSize = new System.Drawing.Size(2068, 1068);
			this.Controls.Add(this.splitContainer1);
			this.Controls.Add(this.panel1);
			this.Controls.Add(this.statusStrip1);
			this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.1F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.MinimumSize = new System.Drawing.Size(2100, 1000);
			this.Name = "Form1";
			this.Text = "Test Cases";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.statusStrip1.ResumeLayout(false);
			this.statusStrip1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.messagesGridView)).EndInit();
			this.panel1.ResumeLayout(false);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			this.panel4.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dictGridView)).EndInit();
			this.panel2.ResumeLayout(false);
			this.panel2.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.ordersGridView)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

    }

        #endregion

        private System.Windows.Forms.Button btnOrders;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.DataGridView messagesGridView;
        private System.Windows.Forms.RichTextBox folderBox;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.DataGridView ordersGridView;
    private System.Windows.Forms.Panel panel2;
    private System.Windows.Forms.Panel panel3;
    private System.Windows.Forms.Panel panel4;
    private System.Windows.Forms.DataGridView dictGridView;
    private System.Windows.Forms.DataGridViewTextBoxColumn FieldTag;
    private System.Windows.Forms.DataGridViewTextBoxColumn FieldName;
    private System.Windows.Forms.DataGridViewTextBoxColumn FieldValue;
    private System.Windows.Forms.DataGridViewTextBoxColumn FieldExpected;
    private System.Windows.Forms.DataGridViewTextBoxColumn Time;
    private System.Windows.Forms.DataGridViewTextBoxColumn InOut;
    private System.Windows.Forms.DataGridViewTextBoxColumn Message;
    private System.Windows.Forms.DataGridViewTextBoxColumn Scenario;
    private System.Windows.Forms.DataGridViewTextBoxColumn Order;
    private System.Windows.Forms.DataGridViewTextBoxColumn Symbol;
    private System.Windows.Forms.DataGridViewTextBoxColumn MainOrder;
    private System.Windows.Forms.DataGridViewTextBoxColumn Amount;
    private System.Windows.Forms.DataGridViewTextBoxColumn Cmd;
    private System.Windows.Forms.DataGridViewTextBoxColumn Action;
    private System.Windows.Forms.DataGridViewTextBoxColumn OpenPrice;
    private System.Windows.Forms.DataGridViewTextBoxColumn ClosePrice;
    private System.Windows.Forms.DataGridViewTextBoxColumn Comment;
    private System.Windows.Forms.Button btnInstruments;
  }
}

