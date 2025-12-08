def UUID_DIR = UUID.randomUUID().toString()
def url = "https://jenkins.snow1k.com/job/test/${BUILD_NUMBER}/"

pipeline {

    agent any

    stages {

        stage('prepare') {
            steps {
                script {
                    echo "JOB URL: ${url}"

                    sh '''
                        rm -rf build
                        mkdir build
                        cd build
                        cmake ..
                    '''
                }
            }
        }

        stage('build') {
            steps {
                script {
                    sh '''
                        cd build
                        make -j 8
                    '''
                }
            }
        }
    }

    post {
        always {
            script {
                sh 'rm -rf build'
                //rocketSend channel: '#SPDP_builds', message: "Autotest Build Notification - STATUS: ${currentBuild.result} - TAGS: ${tags}, ALLURE result: ${url}"
            }
        }
    }
}