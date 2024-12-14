from flask import Flask, render_template, request, redirect, url_for
import sqlite3

app = Flask(__name__)

# 连接到 SQLite 数据库
def get_db_connection():
    conn = sqlite3.connect('database.db')
    conn.row_factory = sqlite3.Row  # 使得返回的行可以通过列名访问
    return conn

# 创建数据库表（如果不存在）
def init_db():
    conn = get_db_connection()
    conn.execute('''
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            age INTEGER NOT NULL
        );
    ''')
    conn.commit()
    conn.close()

# 主页路由，显示所有用户
@app.route('/')
def index():
    conn = get_db_connection()
    users = conn.execute('SELECT * FROM users').fetchall()
    conn.close()
    return render_template('index.html', users=users)

# 添加用户的路由（处理表单提交）
@app.route('/add', methods=('GET', 'POST'))
def add_user():
    if request.method == 'POST':
        name = request.form['name']
        age = request.form['age']

        # 如果没有提供必需字段，返回错误信息
        if not name or not age:
            return 'Name and Age are required!'

        conn = get_db_connection()
        conn.execute('INSERT INTO users (name, age) VALUES (?, ?)', (name, age))
        conn.commit()
        conn.close()

        return redirect(url_for('index'))
    return render_template('add_user.html')

# 查询用户的路由
@app.route('/search', methods=('GET', 'POST'))
def search_user():
    if request.method == 'POST':
        name = request.form.get('name', '').strip()  # 获取查询字段
        age = request.form.get('age', '').strip()  # 获取查询字段
        
        # 查询数据库
        query = "SELECT * FROM users WHERE 1=1"
        params = []
        
        if name:
            query += " AND name LIKE ?"
            params.append(f"%{name}%")
        
        if age:
            query += " AND age = ?"
            params.append(age)

        conn = get_db_connection()
        users = conn.execute(query, tuple(params)).fetchall()
        conn.close()

        return render_template('search_results.html', users=users)
    
    return render_template('search_user.html')

if __name__ == '__main__':
    init_db()  # 初始化数据库
    app.run(host='0.0.0.0', port=5000)
