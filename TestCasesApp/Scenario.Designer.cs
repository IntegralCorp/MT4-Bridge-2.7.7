namespace TestCasesApp
{
  partial class Scenario
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
      this.components = new System.ComponentModel.Container();
      this.dataGridView1 = new System.Windows.Forms.DataGridView();
      this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
      this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.splitContainer1 = new System.Windows.Forms.SplitContainer();
      this.update = new System.Windows.Forms.Button();
      this.folderBox = new System.Windows.Forms.RichTextBox();
      this.btnOrders = new System.Windows.Forms.Button();
      this.panel1 = new System.Windows.Forms.Panel();
      ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
      this.contextMenuStrip1.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
      this.splitContainer1.Panel1.SuspendLayout();
      this.splitContainer1.Panel2.SuspendLayout();
      this.splitContainer1.SuspendLayout();
      this.panel1.SuspendLayout();
      this.SuspendLayout();
      // 
      // dataGridView1
      // 
      this.dataGridView1.BackgroundColor = System.Drawing.SystemColors.ButtonHighlight;
      this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.dataGridView1.ContextMenuStrip = this.contextMenuStrip1;
      this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
      this.dataGridView1.Location = new System.Drawing.Point(0, 0);
      this.dataGridView1.Margin = new System.Windows.Forms.Padding(2);
      this.dataGridView1.Name = "dataGridView1";
      this.dataGridView1.RowHeadersWidth = 102;
      this.dataGridView1.RowTemplate.Height = 40;
      this.dataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
      this.dataGridView1.Size = new System.Drawing.Size(1002, 486);
      this.dataGridView1.TabIndex = 0;
      // 
      // contextMenuStrip1
      // 
      this.contextMenuStrip1.ImageScalingSize = new System.Drawing.Size(40, 40);
      this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem});
      this.contextMenuStrip1.Name = "contextMenuStrip1";
      this.contextMenuStrip1.Size = new System.Drawing.Size(119, 36);
      this.contextMenuStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.contextMenuStrip1_ItemClicked);
      // 
      // addToolStripMenuItem
      // 
      this.addToolStripMenuItem.Name = "addToolStripMenuItem";
      this.addToolStripMenuItem.Size = new System.Drawing.Size(118, 32);
      this.addToolStripMenuItem.Text = "Add";
      // 
      // splitContainer1
      // 
      this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
      this.splitContainer1.Location = new System.Drawing.Point(0, 0);
      this.splitContainer1.Margin = new System.Windows.Forms.Padding(2);
      this.splitContainer1.Name = "splitContainer1";
      this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
      // 
      // splitContainer1.Panel1
      // 
      this.splitContainer1.Panel1.Controls.Add(this.update);
      this.splitContainer1.Panel1.Controls.Add(this.folderBox);
      this.splitContainer1.Panel1.Controls.Add(this.btnOrders);
      // 
      // splitContainer1.Panel2
      // 
      this.splitContainer1.Panel2.Controls.Add(this.panel1);
      this.splitContainer1.Size = new System.Drawing.Size(1002, 586);
      this.splitContainer1.SplitterDistance = 97;
      this.splitContainer1.SplitterWidth = 3;
      this.splitContainer1.TabIndex = 1;
      // 
      // update
      // 
      this.update.Location = new System.Drawing.Point(7, 47);
      this.update.Name = "update";
      this.update.Size = new System.Drawing.Size(172, 38);
      this.update.TabIndex = 8;
      this.update.Text = "Update Instruments File";
      this.update.UseVisualStyleBackColor = true;
      this.update.Click += new System.EventHandler(this.update_Click);
      // 
      // folderBox
      // 
      this.folderBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.folderBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::TestCasesApp.Properties.Settings.Default, "instruments_folder", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
      this.folderBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.folderBox.Location = new System.Drawing.Point(182, 12);
      this.folderBox.Margin = new System.Windows.Forms.Padding(2);
      this.folderBox.Name = "folderBox";
      this.folderBox.Size = new System.Drawing.Size(403, 32);
      this.folderBox.TabIndex = 7;
      this.folderBox.Text = global::TestCasesApp.Properties.Settings.Default.instruments_folder;
      this.folderBox.TextChanged += new System.EventHandler(this.folderBox_TextChanged);
      // 
      // btnOrders
      // 
      this.btnOrders.AutoSize = true;
      this.btnOrders.Location = new System.Drawing.Point(7, 8);
      this.btnOrders.Margin = new System.Windows.Forms.Padding(2);
      this.btnOrders.Name = "btnOrders";
      this.btnOrders.Size = new System.Drawing.Size(138, 34);
      this.btnOrders.TabIndex = 6;
      this.btnOrders.Text = "Read Files";
      this.btnOrders.UseVisualStyleBackColor = true;
      this.btnOrders.Click += new System.EventHandler(this.btnOrders_Click);
      // 
      // panel1
      // 
      this.panel1.Controls.Add(this.dataGridView1);
      this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
      this.panel1.Location = new System.Drawing.Point(0, 0);
      this.panel1.Margin = new System.Windows.Forms.Padding(2);
      this.panel1.Name = "panel1";
      this.panel1.Size = new System.Drawing.Size(1002, 486);
      this.panel1.TabIndex = 0;
      // 
      // Scenario
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(1002, 586);
      this.Controls.Add(this.splitContainer1);
      this.Margin = new System.Windows.Forms.Padding(2);
      this.Name = "Scenario";
      this.Text = "Instruments";
      this.Load += new System.EventHandler(this.Scenario_Load);
      ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
      this.contextMenuStrip1.ResumeLayout(false);
      this.splitContainer1.Panel1.ResumeLayout(false);
      this.splitContainer1.Panel1.PerformLayout();
      this.splitContainer1.Panel2.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
      this.splitContainer1.ResumeLayout(false);
      this.panel1.ResumeLayout(false);
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.DataGridView dataGridView1;
    private System.Windows.Forms.SplitContainer splitContainer1;
    private System.Windows.Forms.Panel panel1;
    private System.Windows.Forms.RichTextBox folderBox;
    private System.Windows.Forms.Button btnOrders;
    private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
    private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
    private System.Windows.Forms.Button update;
  }
}