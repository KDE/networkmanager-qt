diff --git a/libnm-qt/activeconnection.cpp b/libnm-qt/activeconnection.cpp
index f15ab95..17536f2 100644
--- a/libnm-qt/activeconnection.cpp
+++ b/libnm-qt/activeconnection.cpp
@@ -45,6 +45,10 @@ NetworkManager::ActiveConnectionPrivate::ActiveConnectionPrivate(const QString &
     }
 }
 
+NetworkManager::ActiveConnectionPrivate::~ActiveConnectionPrivate()
+{
+}
+
 NetworkManager::ActiveConnection::State NetworkManager::ActiveConnectionPrivate::convertActiveConnectionState(uint state)
 {
     return (NetworkManager::ActiveConnection::State)state;
diff --git a/libnm-qt/activeconnection.h b/libnm-qt/activeconnection.h
index cd11121..e14c5ff 100644
--- a/libnm-qt/activeconnection.h
+++ b/libnm-qt/activeconnection.h
@@ -55,7 +55,7 @@ public:
     /**
      * Destroys a ActiveConnection object.
      */
-    ~ActiveConnection();
+    virtual ~ActiveConnection();
     /**
      * Return path of the connection object
      */
diff --git a/libnm-qt/activeconnection_p.h b/libnm-qt/activeconnection_p.h
index eb356b6..7b2b2de 100644
--- a/libnm-qt/activeconnection_p.h
+++ b/libnm-qt/activeconnection_p.h
@@ -34,6 +34,7 @@ namespace NetworkManager
     {
     public:
         ActiveConnectionPrivate(const QString &);
+        virtual ~ActiveConnectionPrivate();
         static NetworkManager::ActiveConnection::State convertActiveConnectionState(uint);
         NetworkManager::Settings::Connection * connection;
         QString path;
diff --git a/libnm-qt/vpnconnection.h b/libnm-qt/vpnconnection.h
index b8666a9..79d360a 100644
--- a/libnm-qt/vpnconnection.h
+++ b/libnm-qt/vpnconnection.h
@@ -56,7 +56,7 @@ public:
     /**
      * Destroys a VpnConnection object.
      */
-    ~VpnConnection();
+    virtual ~VpnConnection();
     /**
      * Return the current login banner
      */
